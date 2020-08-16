#ifndef TEST_PARSER_CPP
#define TEST_PARSER_CPP

void testParser()
{
    TEST_CASE("Parser should throw a syntax error");
    // Description: Check if a given TokenStream is valid. Inputs which could produces multiple ASTs such as 1 1 are valid.
    std::vector<std::string> inputVector = {
        // 1.) Everything except (, <identifer>, <integer>
        ")", "lambda", "define", "if",
        // 2.) Everything except ( lambda <identifer> <expression>)
        "(lambda 1 x)", "(lambda lambda x)", "(lambda define x)", "(lambda if x)", "(lambda ) x)", "(lambda x )",
        // 3.) Everything except ( define <identifer> <expression>)
        "(define 1 x)", "(define lambda x)", "(define define x)", "(define if x)", "(define ) x)", "(define x )",
        // 4.) Everything except ( if <expression> <expression> <expression>)
        "(if lambda lambda lambda)", "(if lambda define if)", "(if 0 lambda define)", "(if lambda 0 x)", "(if lambda define 0)", "(if 0 0 0(",
        // 5.) Mixed
        ") (lambda x x)", "lambda (if x x x)", "define (if x x x)", "(if lambda define 0)", "(if 0 0 0(", "((", "()", "((()"};

    for (auto input : inputVector)
    {
        auto tokenStream = Lexer<int>().scan(input);
        ASSERT_THROW(Parser().parse(tokenStream), std::runtime_error);
    }
}

#endif