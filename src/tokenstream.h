#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "token.h"

template <class T>
class TokenStream
{
public:
    TokenStream(const std::vector<Token<T>> &tokens)
        : tokens(tokens),
          tokenIt(this->tokens.begin()),
          tokenItEnd(this->tokens.end()) {}

    const bool hasNext() const noexcept
    {
        return tokenIt < tokenItEnd;
    }

    const Token<T> current() const
    {
        if (hasNext())
        {
            return *tokenIt;
        }
        throw std::runtime_error("no more elements");
    }

    const Token<T> next()
    {
        if (hasNext())
        {
            return *tokenIt++;
        }
        throw std::runtime_error("no more elements");
    }

    const bool match(TokenType type) const
    {
        if (hasNext())
        {
            return current().getType() == type;
        }
        throw std::runtime_error("no more elements");
    }

    void skip()
    {
        if (hasNext())
        {
            tokenIt++;
        }
        else
        {
            throw std::runtime_error("no more elements to skip");
        }
    }

    void expect(TokenType type)
    {
        if (!match(type))
        {
            throw std::runtime_error(
                "Expected token:" + Token<T>::typeToString(type) +
                ", was \"" + Token<T>::typeToString(current().getType()) +
                "\" instead.");
        }
    }

    void expectAndSkip(TokenType type)
    {
        if (match(type))
        {
            skip();
        }
        else
        {
            throw std::runtime_error(
                "Expected token:" + Token<T>::typeToString(type) +
                ", was \"" + Token<T>::typeToString(current().getType()) +
                "\" instead.");
        }
    }

    const Token<T> expectAndNext(TokenType type)
    {
        if (match(type))
        {
            return next();
        }
        throw std::runtime_error(
            "Expected token:" + Token<T>::typeToString(type) +
            ", was \"" + Token<T>::typeToString(current().getType()) +
            "\" instead.");
    }

    const bool skipIfMatch(TokenType type)
    {
        if (match(type))
        {
            skip();
            return true;
        }
        return false;
    }

    void print()
    {
        for (auto &token : tokens)
        {
            std::cout << "Token [" << Token<T>::typeToString(token.getType())
                      << "]:" << std::setw(30 - Token<T>::typeToString(token.getType()).length());
            if (token.getType() == TokenType::INTEGER)
            {
                std::cout << token.getValue();
            }
            else
            {
                std::cout << token.getSymbol();
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<Token<T>> tokens;
    typename std::vector<Token<T>>::iterator tokenIt;
    typename std::vector<Token<T>>::iterator tokenItEnd;
};

#endif