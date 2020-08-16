#ifndef TEST_INTERPRETER_CPP
#define TEST_INTERPRETER_CPP

void testInterpreter()
{
    TEST_CASE("Evaluator returns the correct output");
    // Description: We assume that the parser works correctly otherwise we couldn't evaluate the AST
    std::vector<std::pair<std::string, Value<int>>> inputExpectedVector = {
        // 1.) Integer
        std::make_pair("77", Value<int>(77)),
        // 2.) Lambda without parameter
        std::make_pair("(lambda x x)", Value<int>()),
        // 3.) Addition
        std::make_pair("((+ 5) 6)", Value<int>(11)),
        std::make_pair("((+ ((+ 5) 6)) 7)", Value<int>(18)),
        // 4.) Multiple operators
        std::make_pair("((* ((- ((+ 5) 6)) 7)) 2)", Value<int>(8)),
        std::make_pair("((= ((/ ((% 5) 4)) 1)) 1)", Value<int>(1)),
        // 5.) Define integer
        std::make_pair("(define ten 10)", Value<int>(10)),
        // 6.) Conditions
        std::make_pair("(if 0 10 20)", Value<int>(20)),
        std::make_pair("(if 1 10 10)", Value<int>(10)),
        // 7.) Function with operators
        std::make_pair("(define square"
                       "(lambda x ((* x) x)))",
                       Value<int>()),
        // 8.) Function call
        std::make_pair("(square 7)", Value<int>(49)),
        // 9.) Define within function
        std::make_pair("(define foo (lambda x (define bar x)))", Value<int>()),
        // 10.) Call definied function, which defines x
        std::make_pair("(foo 42)", Value<int>(42)),
        // 11.) Call definied variable within function foo
        std::make_pair("bar", Value<int>(42)),
        // 12.) Define within define (same variable)
        std::make_pair("(define x (define x 2))", Value<int>(2)),
        std::make_pair("x", Value<int>(2)),
        // 13.) Define within define (different variable)
        std::make_pair("(define x (define y 5))", Value<int>(5)),
        std::make_pair("x", Value<int>(5)),
        std::make_pair("y", Value<int>(5)),
        // 14.) Nested function
        std::make_pair("(define sum-of-squares"
                       "(lambda x"
                       "(lambda y"
                       "((+(square x))(square y)))))",
                       Value<int>()),
        // 15.) Call nested function
        std::make_pair("((sum-of-squares 3) 7)", Value<int>(58)),
        // 16.) Define recursive function
        std::make_pair("(define fak (lambda n (if ((= 0) n) 1 ((* n) (fak ((- n) 1))))))", Value<int>()),
        // 17.) Call recursive function
        std::make_pair("(fak 5)", Value<int>(120)),
        // 18.) Define recursive function with 2 recursive function calls
        std::make_pair("(define ggt"
                       "(lambda x "
                       "(lambda y (if ((= x) y) x (if ((< x) y) ((ggt x) ((- y) x)) ((ggt y) ((- x) y)))))))",
                       Value<int>()),
        // 19.) Call recursive function with 2 recursive function calls
        std::make_pair("((ggt 366) 671)", Value<int>(61)),
        // 20.) Define multiple functions
        std::make_pair("(define checkfactors"
                       "(lambda n (lambda f (if ((> f) 1) (if ((% n) f) ((checkfactors n) ((- f) 1)) 0 ) 1 ) )))",
                       Value<int>()),
        std::make_pair("(define isprime (lambda n (if ((= n) 2) 1 (if ((< n) 2) 0 ((checkfactors n) ((- n) 1)) ) ) ))", Value<int>()),
        std::make_pair("(define seekprime (lambda n (if (isprime n) n (seekprime ((+ n) 1))) ))", Value<int>()),
        std::make_pair("(define nextprime (lambda n (if ((< n) 2) 2 (seekprime ((+ n) 1))) ))", Value<int>()),
        // 21.) Call multiple functions
        std::make_pair("(nextprime 1)", Value<int>(2)),
        std::make_pair("(nextprime 2)", Value<int>(3)),
        std::make_pair("(nextprime 3)", Value<int>(5)),
        std::make_pair("(nextprime 5)", Value<int>(7)),
        std::make_pair("(nextprime 7)", Value<int>(11)),
        std::make_pair("(nextprime 11)", Value<int>(13)),
        std::make_pair("(nextprime 13)", Value<int>(17)),
        // 22.) Define function which passes another function as a parameter
        std::make_pair("(define repeat (lambda n (lambda f (lambda val (if ((> n) 0) (((repeat ((- n) 1)) f) (f val)) val) ))))", Value<int>()),
        // 23.) Call function
        std::make_pair("(((repeat 100) nextprime) 1)", Value<int>(541)),
        // 24.) Redefine +
        std::make_pair("(define + (lambda a (lambda b ((* a ) b ))))", Value<int>()),
        // 25.) Call redefined +
        std::make_pair("((+ 2) 9)", Value<int>(18))};

    std::shared_ptr<Environment<int>>
        globalEnv = std::make_shared<Environment<int>>();
    for (auto pair : inputExpectedVector)
    {
        auto ast = Parser().parse(Lexer<int>().scan(pair.first));
        SemanticAnalyzer<int>(globalEnv).analyse(ast);
        auto result = Evaluator<int>(globalEnv).evaluate(ast);
        if (result.getType() == ValueType::INTEGER)
        {
            ASSERT_EQUAL(result.getValue(), pair.second.getValue());
        }
        else
        {
            ASSERT_EQUAL(static_cast<int>(result.getType()), static_cast<int>(pair.second.getType()));
        }
    }

    TEST_CASE("Evaluator can use predefinied Value");

    std::map<std::string, Value<int>> predefinied =
        {{"...", Value<int>([](int n, int m) -> int {
              int result = 0;
              while (m > 0)
              {
                  m -= 1;
                  result += n;
              }
              return result;
          })},
         {"inv", Value<int>([](int n) -> int {
              return -n;
          })}};

    std::vector<char> characterSet = {'.'};

    std::vector<std::pair<std::string, Value<int>>> inputExpectedVector2 = {
        // 1.) Use predefinied function
        std::make_pair("inv", Value<int>()),
        std::make_pair("((... 4) 2)", Value<int>(8)),
        // 2.) Use predefinied function with another function
        std::make_pair("((* ((... 4) 2)) 5)", Value<int>(40)),
        std::make_pair("(inv 10)", Value<int>(-10))};

    std::shared_ptr<Environment<int>>
        globalEnv2 = std::make_shared<Environment<int>>(predefinied);
    for (auto pair : inputExpectedVector2)
    {
        auto ast = Parser().parse(Lexer<int>(characterSet).scan(pair.first));
        SemanticAnalyzer<int>(globalEnv2).analyse(ast);
        auto result = Evaluator<int>(globalEnv2).evaluate(ast);
        if (result.getType() == ValueType::INTEGER)
        {
            ASSERT_EQUAL(result.getValue(), pair.second.getValue());
        }
        else
        {
            ASSERT_EQUAL(static_cast<int>(result.getType()), static_cast<int>(pair.second.getType()));
        }
    }

    TEST_CASE("Evaluate invalid node");

    std::shared_ptr<Node> unexpectedNode = std::make_shared<Node>();
    ASSERT_THROW(Evaluator<int>(globalEnv2).evaluate(unexpectedNode), std::runtime_error);

    TEST_CASE("Call integer/Get integer from a value");

    Value<int> param(1);
    Value<int> invalidValue(1);
    ASSERT_THROW(invalidValue.callFunction(param), std::runtime_error);
    invalidValue = Value<int>();
    ASSERT_THROW(invalidValue.getValue(), std::runtime_error);
    invalidValue = Value<int>(Value<int>([](Value<int> a) {
        return Value<int>();
    }));
    ASSERT_THROW(invalidValue.getValue(), std::runtime_error);

    TEST_CASE("Lookup undefinied symbol");
    std::shared_ptr<Environment<int>> globalEnv3 = std::make_shared<Environment<int>>();
    ASSERT_THROW(globalEnv3->lookup("e"), std::runtime_error);
}

#endif