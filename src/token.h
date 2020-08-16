#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <stdexcept>

enum class TokenType
{
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    IDENTIFIER,
    INTEGER,
    LAMBDA,
    DEFINE,
    IF
};

template <class T>
class Token
{

public:
    Token(T value) : value(value), type(TokenType::INTEGER) {}
    Token(TokenType type) : symbol(typeToString(type)), value(0), type(type) {}
    Token(std::string symbol) : symbol(symbol), type(TokenType::IDENTIFIER) {}

    const TokenType getType() const
    {
        return type;
    }

    const std::string getSymbol() const
    {
        if (type != TokenType::INTEGER)
        {
            return symbol;
        }
        else
        {
            throw std::runtime_error("Token is an integer");
        }
    }

    const T getValue() const
    {
        if (type == TokenType::INTEGER)
        {
            return value;
        }
        else
        {
            throw std::runtime_error("Token is not an integer");
        }
    }

    const static std::string typeToString(TokenType type)
    {
        switch (type)
        {
        case TokenType::LEFT_PARENTHESIS:
            return "(";
        case TokenType::RIGHT_PARENTHESIS:
            return ")";
        case TokenType::INTEGER:
            return "integer";
        case TokenType::LAMBDA:
            return "lambda";
        case TokenType::DEFINE:
            return "define";
        case TokenType::IF:
            return "if";
        default:
            return "identifier";
        }
    }

private:
    std::string symbol;
    T value;
    TokenType type;
};

#endif