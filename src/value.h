#ifndef VALUE_H
#define VALUE_H

#include <functional>

enum class ValueType
{
    INTEGER,
    FUNCTION
};

template <class T>
class Value
{

public:
    Value() : function(nullptr),
              type(ValueType::FUNCTION) {}

    Value(const T &value) : value(value), type(ValueType::INTEGER) {}

    Value(const std::function<Value<T>(Value<T>)> function)
        : function(function), type(ValueType::FUNCTION) {}

    Value(const std::function<T(T)> unaryFunction)
        : function([=](Value<T> parameter) -> Value<T> {
              return Value<T>(unaryFunction(parameter.getValue()));
          }),
          type(ValueType::FUNCTION) {}

    Value(const std::function<T(T, T)> binaryFunction)
        : function([=](Value<T> parameter1) -> Value<T> {
              // Returns a Value<T> which contains other function
              return Value<T>([=](Value<T> parameter2) mutable -> Value<T> {
                  // Applies the two parameters
                  // to std::function<T(T, T)> function
                  return Value<T>(binaryFunction(parameter1.getValue(), parameter2.getValue()));
              });
          }),
          type(ValueType::FUNCTION) {}

    const Value<T> callFunction(Value<T> &parameter) const
    {
        if (type == ValueType::FUNCTION)
        {
            return function(parameter);
        }
        throw std::runtime_error("Value is not a function");
    }

    const T getValue() const
    {
        if (type == ValueType::INTEGER)
        {
            return value;
        }
        throw std::runtime_error("Value is not an integer");
    }

    const ValueType getType() const
    {
        return type;
    }

private:
    T value;
    std::function<Value<T>(Value<T>)> function;
    ValueType type;
};

#endif