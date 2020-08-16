#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "semanticanalyzer.h"

template <class T>
class Evaluator
{

public:
    Evaluator(std::shared_ptr<Environment<T>> &globalEnv) : globalEnv(globalEnv)
    {
    }

    const Value<T> evaluate(const std::shared_ptr<Node> &node) const
    {
        return evaluateAST(node, globalEnv);
    }

private:
    const Value<T> evaluateAST(
        const std::shared_ptr<Node> &node,
        const std::shared_ptr<Environment<T>> &env) const
    {
        if (auto functionNode = std::dynamic_pointer_cast<FunctionNode>(node))
        {
            auto function = evaluateAST(functionNode->function(), env);
            auto parameter = evaluateAST(functionNode->parameter(), env);
            return function.callFunction(parameter);
        }
        else if (auto lambdaNode = std::dynamic_pointer_cast<LambdaNode>(node))
        {
            auto identifier = lambdaNode->identifier();
            auto expr = lambdaNode->expression();
            return Value<T>([=](Value<T> parameter) mutable -> Value<T> {
                auto newEnvironment = std::make_shared<Environment<T>>(env);
                newEnvironment->insert(identifier, parameter);
                return evaluateAST(expr, newEnvironment);
            });
        }
        else if (auto assignmentNode = std::dynamic_pointer_cast<AssignmentNode>(node))
        {
            auto identifier = assignmentNode->identifier();
            Value<T> expr = evaluateAST(assignmentNode->expression(), env);
            globalEnv->insert(identifier, expr);
            return env->lookup(identifier);
        }
        else if (auto conditionNode = std::dynamic_pointer_cast<ConditionNode>(node))
        {
            auto result = evaluateAST(conditionNode->condition(), env);
            if (result.getValue() != 0)
            {
                return evaluateAST(conditionNode->expression(), env);
            }
            else
            {
                return evaluateAST(conditionNode->altExpression(), env);
            }
        }
        else if (auto identifierNode = std::dynamic_pointer_cast<IdentifierNode>(node))
        {
            auto identifier = identifierNode->identifier();
            return env->lookup(identifier);
        }
        else if (auto integerNode = std::dynamic_pointer_cast<IntegerNode<T>>(node))
        {
            return Value<T>(integerNode->integer());
        }
        else
        {
            throw std::runtime_error("Unexpected node");
        }
    }

    std::shared_ptr<Environment<T>> &globalEnv;
};

#endif