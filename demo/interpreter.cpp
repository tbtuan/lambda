#include "../src/evaluator.h"

int main()
{
    std::string input = "";
    std::cout << "> ";
    // A predefinied ! function (in this case a factorial function)
    std::map<std::string, Value<int>> predefinied =
        {{"!", Value<int>([](int n) -> int {
              int result = 1;
              while (n > 1)
              {
                  result *= n;
                  n -= 1;
              }
              return result;
          })}};
    // An identifier ! function
    std::vector<char> characterSet = {'!'};

    // Interpreter
    std::shared_ptr<Environment<int>> globalEnv = std::make_shared<Environment<int>>(predefinied);
    while (std::getline(std::cin, input))
    {
        auto tokenstream = Lexer<int>(characterSet).scan(input);
        auto ast = Parser().parse(tokenstream);
        SemanticAnalyzer<int>(globalEnv).analyse(ast);
        auto result = Evaluator<int>(globalEnv).evaluate(ast);
        if (result.getType() == ValueType::INTEGER)
        {
            std::cout << result.getValue() << std::endl;
        }
    }
}