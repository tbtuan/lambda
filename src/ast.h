
#ifndef AST_H
#define AST_H

#include <memory>

class Node
{
public:
    virtual ~Node() = default;
};

class FunctionNode : public Node
{
public:
    FunctionNode(const std::shared_ptr<Node> &func, const std::shared_ptr<Node> &param)
        : func(func), param(param) {}

    const std::shared_ptr<Node> function() const
    {
        return func;
    }

    const std::shared_ptr<Node> parameter() const
    {
        return param;
    }

private:
    const std::shared_ptr<Node> func;
    const std::shared_ptr<Node> param;
};

class AssignmentNode : public Node
{
public:
    AssignmentNode(const std::string id, const std::shared_ptr<Node> &expr)
        : id(id), expr(expr) {}

    const std::string identifier() const
    {
        return id;
    }

    const std::shared_ptr<Node> expression() const
    {
        return expr;
    }

private:
    const std::string id;
    const std::shared_ptr<Node> expr;
};

class LambdaNode : public Node
{
public:
    LambdaNode(const std::string id, std::shared_ptr<Node> expr)
        : id(id), expr(expr) {}

    const std::string identifier() const
    {
        return id;
    }

    const std::shared_ptr<Node> expression() const
    {
        return expr;
    }

private:
    const std::string id;
    const std::shared_ptr<Node> expr;
};

class ConditionNode : public Node
{
public:
    ConditionNode(
        const std::shared_ptr<Node> &cond,
        const std::shared_ptr<Node> &expr,
        const std::shared_ptr<Node> &altExpr)
        : cond(cond), expr(expr), altExpr(altExpr) {}

    const std::shared_ptr<Node> condition() const
    {
        return cond;
    }

    const std::shared_ptr<Node> expression() const
    {
        return expr;
    }

    const std::shared_ptr<Node> altExpression() const
    {
        return altExpr;
    }

private:
    const std::shared_ptr<Node> cond;
    const std::shared_ptr<Node> expr;
    const std::shared_ptr<Node> altExpr;
};

class IdentifierNode : public Node
{
public:
    IdentifierNode(const std::string id) : id(id) {}

    const std::string identifier() const
    {
        return id;
    }

private:
    const std::string id;
};

template <class T>
class IntegerNode : public Node
{
public:
    IntegerNode(const T value) : value(value) {}

    const T integer() const
    {
        return value;
    }

private:
    const T value;
};

#endif