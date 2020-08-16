#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>
#include <map>
#include <memory>
#include <algorithm>

#include "token.h"
#include "tokenstream.h"

template <class T>
class Lexer
{
public:
    Lexer() {}

    Lexer(std::vector<char> identifiers)
    {
        this->identifiers.insert(this->identifiers.begin(), identifiers.begin(), identifiers.end());
    }

    Lexer(std::function<T(std::string)> converter)
    {
        this->converter = converter;
    }

    Lexer(std::vector<char> identifiers, std::function<T(std::string)> converter)
    {
        this->identifiers.insert(this->identifiers.begin(), identifiers.begin(), identifiers.end());
        this->converter = converter;
    }

    const std::shared_ptr<TokenStream<T>> scan(const std::string &input)
    {
        std::vector<Token<T>> tokens;
        size_t i = 0;
        size_t n = input.length();

        while (i < n)
        {
            if (input[i] == '(')
            {
                tokens.push_back(Token<T>(TokenType::LEFT_PARENTHESIS));
                i++;
            }
            else if (input[i] == ')')
            {
                tokens.push_back(Token<T>(TokenType::RIGHT_PARENTHESIS));
                i++;
            }
            else if (std::isspace(input[i]))
            {
                i++;
            }
            else
            {
                // Keyword/Identifier/etc
                std::string stringBuilder = "";
                while (i < n)
                {
                    if (std::isspace(input[i]) || input[i] == '(' || input[i] == ')')
                    {
                        break;
                    }
                    else
                    {
                        stringBuilder += input[i];
                    }
                    i++;
                }
                if (isInteger(stringBuilder))
                {
                    tokens.push_back(Token<T>(converter(stringBuilder)));
                }
                else if (stringBuilder == "lambda")
                {
                    tokens.push_back(Token<T>(TokenType::LAMBDA));
                }
                else if (stringBuilder == "define")
                {
                    tokens.push_back(Token<T>(TokenType::DEFINE));
                }
                else if (stringBuilder == "if")
                {
                    tokens.push_back(Token<T>(TokenType::IF));
                }
                else if (isIdentifier(stringBuilder))
                {
                    tokens.push_back(Token<T>(stringBuilder));
                }
                else
                {
                    throw std::runtime_error("Invalid token:" + stringBuilder);
                }
            }
        }
        return std::make_shared<TokenStream<T>>(tokens);
    }

private:
    const bool isIdentifier(const std::string &input) const
    {
        for (size_t i = 0; i < input.length(); i++)
        {
            if ((!(input[i] >= 'A' && input[i] <= 'Z') &&
                 !(input[i] >= 'a' && input[i] <= 'z')) &&
                (std::find(identifiers.begin(), identifiers.end(), input[i]) == identifiers.end()))
            {
                return false;
            }
        }
        return true;
    }

    const bool isInteger(const std::string &input) const
    {
        for (size_t i = 0; i < input.length(); i++)
        {
            if (!std::isdigit(input[i]))
            {
                // Positive/negative numbers with e. g. -432 -21 -1 and even -01, 01 should work
                // "-", "1-" are invalid
                if (i == 0 && input[i] == '-' && input.length() > 1)
                {
                    continue;
                }
                return false;
            }
        }
        return true;
    }

private:
    std::function<T(std::string)> converter = [](std::string input) -> T {
        return std::stoi(input);
    };
    std::vector<char> identifiers = {'+', '-', '/', '*', '<', '=', '>', '%'};
};

#endif