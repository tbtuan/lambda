#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <iostream>
#include "lexer.h"
#include "ast.h"

class Parser
{

    // BNF
    // <EXPR> -> <IDENTIFIER>                           (1)
    //        -> <INTEGER>                              (2)
    //        -> "(" <EXPR> <EXPR> ")"                  (3)
    //        -> "(" lambda <IDENTIFIER> <EXPR> ")"     (4)
    //        -> "(" define <IDENTIFIER> <EXPR> ")"     (5)
    //        -> "(" if <EXPR> <EXPR> <EXPR> ")"        (6)

public:
    template <class T>
    const std::shared_ptr<Node> parse(const std::shared_ptr<TokenStream<T>> &tokenStream)
    {
        if (tokenStream->skipIfMatch(TokenType::LEFT_PARENTHESIS))
        {
            // Expression: ( lambda <EXPR> )
            if (tokenStream->skipIfMatch(TokenType::LAMBDA))
            {
                auto identifier = tokenStream->expectAndNext(TokenType::IDENTIFIER).getSymbol();
                auto expr = parse(tokenStream);
                tokenStream->expectAndSkip(TokenType::RIGHT_PARENTHESIS);

                return std::make_shared<LambdaNode>(identifier, expr);
            }
            // Expression: ( define <IDENTIFIER> <EXPR> )
            else if (tokenStream->skipIfMatch(TokenType::DEFINE))
            {
                auto identifier = tokenStream->expectAndNext(TokenType::IDENTIFIER).getSymbol();
                auto expr = parse(tokenStream);
                tokenStream->expectAndSkip(TokenType::RIGHT_PARENTHESIS);

                return std::make_shared<AssignmentNode>(identifier, expr);
            }
            // Expression: ( if <EXPR> <EXPR> <EXPR> )
            else if (tokenStream->skipIfMatch(TokenType::IF))
            {
                auto condition = parse(tokenStream);
                auto expr1 = parse(tokenStream);
                auto expr2 = parse(tokenStream);
                tokenStream->expectAndSkip(TokenType::RIGHT_PARENTHESIS);

                return std::make_shared<ConditionNode>(condition, expr1, expr2);
            }
            // Expression: ( <EXPR> <EXPR> )
            else
            {
                auto expr1 = parse(tokenStream);
                auto expr2 = parse(tokenStream);
                tokenStream->expectAndSkip(TokenType::RIGHT_PARENTHESIS);

                return std::make_shared<FunctionNode>(expr1, expr2);
            }
        }
        // Expression: <IDENTIFIER>
        else if (tokenStream->match(TokenType::IDENTIFIER))
        {
            return std::make_shared<IdentifierNode>(tokenStream->next().getSymbol());
        }
        // Expression: <INTEGER>
        else if (tokenStream->match(TokenType::INTEGER))
        {
            return std::make_shared<IntegerNode<T>>(tokenStream->next().getValue());
        }
        else
        {
            throw std::runtime_error("Expected token: \"(\", <identifier>, <operator> or <integer>");
        }
    }
};

#endif