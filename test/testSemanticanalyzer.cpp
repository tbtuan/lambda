#ifndef TEST_SEMANTICANALYZER_CPP
#define TEST_SEMANTICANALYZER_CPP

void testSemanticanalyzer()
{
    TEST_CASE("SemanticAnalyzer should throw a semantic error");
    // Description: We assume that the parser works correctly otherwise we couldn't evaluate the AST
    std::vector<std::string> inputVector = {
        // 1.) Undefinied identifier
        "x",
        // 2.) Undefinied within lambda
        "(lambda x y)", "(lambda x (lambda y z))",
        // 3.) Undefinied within define
        "(define x y)", "(define x (define x y))",
        // 4.) Undefinied within if
        "(if z 0 0)", "(if 0 z 0)", "(if 0 0 z)",
        // 5.) Mixed
        "(if (define x y) 0 0)", "(if (define x (lambda b y)) 0 0)", "(define x (lambda b (define y (lambda c a))))"};

    for (auto input : inputVector)
    {
        std::shared_ptr<Environment<int>> globalEnv = std::make_shared<Environment<int>>();
        ASSERT_THROW(SemanticAnalyzer<int>(globalEnv).analyse(Parser().parse(Lexer<int>().scan(input))), std::runtime_error);
    }

    std::shared_ptr<Node> unexpectedNode = std::make_shared<Node>();
    std::shared_ptr<Environment<int>> globalEnv = std::make_shared<Environment<int>>();
    ASSERT_THROW(SemanticAnalyzer<int>(globalEnv).analyse(unexpectedNode), std::runtime_error);
}

#endif