#ifndef TEST_LEXER_CPP
#define TEST_LEXER_CPP

void testLexer()
{
    // Test case
    TEST_CASE("Lexer should recognize delimiters, integers, keywords and identifiers");
    std::string input =
        // 1.) Delimiters "(",")"," ", "\n" next to each other
        "(\n) \n\n"
        // 2.) Delimiters between keywords
        "lambda\n  if)define("
        // 3.) Delimiters between integers + some valid integers
        "\n1234567890 -01 01 (2)-200 "
        // 4.) identifiers and operators
        "lambdaif(x)\n-% -(+";
    auto tokenstream = Lexer<int>().scan(input);
    std::vector<Token<int>> tokens{
        // 1.)
        Token<int>(TokenType::LEFT_PARENTHESIS),
        Token<int>(TokenType::RIGHT_PARENTHESIS),
        // 2.)
        Token<int>(TokenType::LAMBDA),
        Token<int>(TokenType::IF),
        Token<int>(TokenType::RIGHT_PARENTHESIS),
        Token<int>(TokenType::DEFINE),
        Token<int>(TokenType::LEFT_PARENTHESIS),
        // 3.)
        Token<int>(1234567890),
        Token<int>(-1),
        Token<int>(1),
        Token<int>(TokenType::LEFT_PARENTHESIS),
        Token<int>(2),
        Token<int>(TokenType::RIGHT_PARENTHESIS),
        Token<int>(-200),
        // 4.)
        Token<int>("lambdaif"),
        Token<int>(TokenType::LEFT_PARENTHESIS),
        Token<int>("x"),
        Token<int>(TokenType::RIGHT_PARENTHESIS),
        Token<int>("-%"),
        Token<int>("-"),
        Token<int>(TokenType::LEFT_PARENTHESIS),
        Token<int>("+")};
    for (auto token : tokens)
    {
        if (token.getType() == TokenType::INTEGER)
        {
            ASSERT_EQUAL(tokenstream->current().getValue(), token.getValue());
        }
        else
        {
            ASSERT_EQUAL(Token<int>::typeToString(tokenstream->current().getType()), Token<int>::typeToString(token.getType()));
            ASSERT_EQUAL(tokenstream->current().getSymbol(), token.getSymbol());
        }
        tokenstream->skip();
    }

    ASSERT_THROW(Lexer<int>().scan("invalid?"), std::runtime_error);
    ASSERT_THROW(Lexer<int>().scan("(invalid ?"), std::runtime_error);

    TEST_CASE("TokenStream methods work");

    std::vector<Token<int>> tokens2 = {Token<int>(TokenType::DEFINE),
                                       Token<int>(TokenType::LAMBDA),
                                       Token<int>(TokenType::LEFT_PARENTHESIS),
                                       Token<int>(TokenType::RIGHT_PARENTHESIS),
                                       Token<int>(TokenType::INTEGER),
                                       Token<int>(TokenType::IF),
                                       Token<int>("test"),
                                       Token<int>(5)};
    auto tokenStream2 = TokenStream<int>(tokens2);
    // call current
    ASSERT_EQUAL(tokenStream2.current().getSymbol(), "define");
    ASSERT_EQUAL(Token<int>::typeToString(tokenStream2.current().getType()), Token<int>::typeToString(TokenType::DEFINE));
    // call next (next returns define and iterates once, an ASSERT FAILTURE WILL CALL IT TWICE)
    ASSERT_EQUAL(tokenStream2.next().getSymbol(), "define");
    ASSERT_EQUAL(Token<int>::typeToString(tokenStream2.current().getType()), Token<int>::typeToString(TokenType::LAMBDA));
    ASSERT_EQUAL(tokenStream2.current().getSymbol(), "lambda");
    ASSERT_EQUAL(Token<int>::typeToString(tokenStream2.current().getType()), Token<int>::typeToString(TokenType::LAMBDA));
    ASSERT_CALL(tokenStream2.skip());
    ASSERT_CALL(tokenStream2.expect(TokenType::LEFT_PARENTHESIS));
    ASSERT_EQUAL(tokenStream2.current().getSymbol(), "(");
    ASSERT_EQUAL(Token<int>::typeToString(tokenStream2.current().getType()), Token<int>::typeToString(TokenType::LEFT_PARENTHESIS));
    ASSERT_EQUAL(tokenStream2.hasNext(), true);
    ASSERT_EQUAL(tokenStream2.match(TokenType::LEFT_PARENTHESIS), true);
    ASSERT_EQUAL(tokenStream2.skipIfMatch(TokenType::RIGHT_PARENTHESIS), false);
    ASSERT_EQUAL(tokenStream2.skipIfMatch(TokenType::LEFT_PARENTHESIS), true);
    ASSERT_THROW(tokenStream2.expect(TokenType::INTEGER), std::runtime_error);
    ASSERT_THROW(tokenStream2.expectAndSkip(TokenType::INTEGER), std::runtime_error);
    ASSERT_THROW(tokenStream2.expectAndNext(TokenType::INTEGER), std::runtime_error);
    ASSERT_CALL(tokenStream2.skip());
    ASSERT_EQUAL(tokenStream2.expectAndNext(TokenType::INTEGER).getValue(), 0);
    ASSERT_EQUAL(tokenStream2.next().getSymbol(), "if");
    ASSERT_EQUAL(tokenStream2.next().getSymbol(), "test");
    ASSERT_EQUAL(tokenStream2.current().getValue(), 5);
    ASSERT_CALL(tokenStream2.skip());
    // No elements
    ASSERT_THROW(tokenStream2.skip(), std::runtime_error);
    ASSERT_THROW(tokenStream2.current(), std::runtime_error);
    ASSERT_THROW(tokenStream2.next(), std::runtime_error);
    ASSERT_THROW(tokenStream2.match(TokenType::LEFT_PARENTHESIS), std::runtime_error);
    ASSERT_EQUAL(tokenStream2.hasNext(), false);
    ASSERT_CALL(tokenStream2.print());

    TEST_CASE("Converter function should work");
    // The converter can be rewritten
    // 2147483628 = (2^32 / 2) + 1 = max 32 bit signed int + 1
    auto tokenstream3 = Lexer<long>([](std::string input) -> long {
                            return std::stol(input);
                        }).scan("2147483628");
    int result3 = tokenstream3->next().getValue();
    // If it doesn't work there would be an overflow (of course it depends on the underlying architecture)
    ASSERT_EQUAL(result3, 2147483628);

    TEST_CASE("Lexer can be extended");
    std::vector<char> characterSet = {'#', '_', '.', ':', ';', '\'', '0', '3', '4', '5'};
    auto tokenstream4 = Lexer<long>(characterSet).scan("4s: yo# can s33 i'm u5ing ;Semicolons; ev.n n_mbers w0rk 1234");
    std::vector<Token<int>> expectedTokens4{
        Token<int>("4s:"),
        Token<int>("yo#"),
        Token<int>("can"),
        Token<int>("s33"),
        Token<int>("i'm"),
        Token<int>("u5ing"),
        Token<int>(";Semicolons;"),
        Token<int>("ev.n"),
        Token<int>("n_mbers"),
        Token<int>("w0rk"),
        Token<int>(1234)};
    for (auto token : expectedTokens4)
    {
        if (token.getType() == TokenType::INTEGER)
        {
            ASSERT_EQUAL(tokenstream4->current().getValue(), token.getValue());
        }
        else
        {
            ASSERT_EQUAL(tokenstream4->current().getSymbol(), token.getSymbol());
        }
        tokenstream4->skip();
    }

    TEST_CASE("Cannot retrieve value/symbol within a Token");
    Token<int> invalidToken(1);
    ASSERT_THROW(invalidToken.getSymbol(), std::runtime_error);
    invalidToken = Token<int>(TokenType::LAMBDA);
    ASSERT_THROW(invalidToken.getValue(), std::runtime_error);
}

#endif