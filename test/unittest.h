// A minimalistic "test suite"

// Macro definition for the tests
#define TEST_CASE(name) printTestcase(name)

inline void printTestcase(std::string name)
{
    std::cout << "===============================================================================" << std::endl;
    std::cout << "[" << name << "]" << std::endl;
    std::cout << "===============================================================================" << std::endl;
}

// Based on
// https://stackoverflow.com/questions/32100626/how-to-assert-in-cppunit-that-a-statement-throws-an-exception-either-of-type-exc
// Executes code and validates the type of an exception with ExceptionType
#define ASSERT_THROW(code, ExceptionType)                                                 \
    do                                                                                    \
    {                                                                                     \
        std::string message;                                                              \
        try                                                                               \
        {                                                                                 \
            code;                                                                         \
            std::cerr << "Test failed: No exceptions were thrown "                        \
                      << ", in file:" << __FILE__ << ", line: " << __FILE__ << std::endl; \
            std::abort();                                                                 \
        }                                                                                 \
        catch (ExceptionType const &ex)                                                   \
        {                                                                                 \
            std::cout << "Test sucessful: "                                               \
                      << "\"" << ex.what() << "\"" << std::endl;                          \
        }                                                                                 \
        catch (...)                                                                       \
        {                                                                                 \
            std::cerr << "Test failed: A different exception was thrown "                 \
                      << ", in file:" << __FILE__ << ", line: " << __FILE__ << std::endl; \
            std::abort();                                                                 \
        }                                                                                 \
    } while (false)

#define ASSERT_CALL(code)                                                                 \
    do                                                                                    \
    {                                                                                     \
        std::string message;                                                              \
        try                                                                               \
        {                                                                                 \
            code;                                                                         \
            std::cout << "Test sucessful: Code executed" << std::endl;                    \
        }                                                                                 \
        catch (...)                                                                       \
        {                                                                                 \
            std::cerr << "Test failed: An exception was thrown "                          \
                      << ", in file:" << __FILE__ << ", line: " << __FILE__ << std::endl; \
            std::abort();                                                                 \
        }                                                                                 \
    } while (false)

#define ASSERT_EQUAL(codeActual, codeExpected)                                                \
    do                                                                                        \
    {                                                                                         \
        bool condition = false;                                                               \
        try                                                                                   \
        {                                                                                     \
            condition = (codeActual == codeExpected);                                         \
            if (condition)                                                                    \
            {                                                                                 \
                std::cout << "Test successful: " << codeExpected << std::endl;                \
            }                                                                                 \
            else                                                                              \
            {                                                                                 \
                std::cerr << "Test failed: "                                                  \
                             "expected \""                                                    \
                          << codeExpected << "\" was "                                        \
                          << "\"" << codeActual << "\" instead"                               \
                          << ", in file:" << __FILE__ << ", line: " << __FILE__ << std::endl; \
                std::abort();                                                                 \
            }                                                                                 \
        }                                                                                     \
        catch (...)                                                                           \
        {                                                                                     \
            std::cerr << "Test failed: unexpected exception "                                 \
                      << ", in file:" << __FILE__ << ", line: " << __FILE__ << std::endl;     \
            std::abort();                                                                     \
        }                                                                                     \
    } while (false)
