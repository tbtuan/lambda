#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <vector>
#include <memory>
#include <set>
#include "parser.h"
#include "value.h"
#include "environment.h"

template <class T>
class SemanticAnalyzer
{

public:
    SemanticAnalyzer(std::shared_ptr<Environment<T>> &globalEnv) : globalEnv(globalEnv)
    {
    }

    void analyse(const std::shared_ptr<Node> &node) const
    {
        analyseSemantic(node, globalEnv);
    }

private:
    void analyseSemantic(
        const std::shared_ptr<Node> &node,
        const std::shared_ptr<Environment<T>> &env) const
    {
        if (auto functionNode = std::dynamic_pointer_cast<FunctionNode>(node))
        {
            analyseSemantic(functionNode->function(), env);
            analyseSemantic(functionNode->parameter(), env);
        }
        else if (auto lambdaNode = std::dynamic_pointer_cast<LambdaNode>(node))
        {
            auto identifier = lambdaNode->identifier();
            auto newEnvironment = std::make_shared<Environment<T>>(env);
            newEnvironment->insert(identifier, Value<T>());
            analyseSemantic(lambdaNode->expression(), newEnvironment);
        }
        else if (auto assignmentNode = std::dynamic_pointer_cast<AssignmentNode>(node))
        {
            auto identifier = assignmentNode->identifier();
            globalEnv->insert(identifier, Value<T>());
            analyseSemantic(assignmentNode->expression(), env);
        }
        else if (auto conditionNode = std::dynamic_pointer_cast<ConditionNode>(node))
        {
            analyseSemantic(conditionNode->condition(), env);
            analyseSemantic(conditionNode->expression(), env);
            analyseSemantic(conditionNode->altExpression(), env);
        }
        else if (auto identifierNode = std::dynamic_pointer_cast<IdentifierNode>(node))
        {
            auto identifier = identifierNode->identifier();
            env->lookup(identifier);
        }
        else if (auto integerNode = std::dynamic_pointer_cast<IntegerNode<T>>(node))
        {
            return;
        }
        else
        {
            throw std::runtime_error("Unexpected node");
        }
    }

    std::shared_ptr<Environment<T>> &globalEnv;
};

#endif