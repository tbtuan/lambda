#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <memory>
#include "value.h"

template <class T>
class Environment
{

public:
    Environment()
        : outerEnvironment(nullptr),
          symbolTable()
    {
        insert("+", Value<T>([](T a, T b) -> T {
                   return a + b;
               }));
        insert("-", Value<T>([](T a, T b) -> T {
                   return a - b;
               }));
        insert("*", Value<T>([](T a, T b) -> T {
                   return a * b;
               }));
        insert("/", Value<T>([](T a, T b) -> T {
                   return a / b;
               }));
        insert("%", Value<T>([](T a, T b) -> T {
                   return a % b;
               }));
        insert("<", Value<T>([](T a, T b) -> T {
                   return a < b;
               }));
        insert(">", Value<T>([](T a, T b) -> T {
                   return a > b;
               }));
        insert("=", Value<T>([](T a, T b) -> T {
                   return a == b;
               }));
    }

    Environment(const std::map<std::string, Value<T>> &symbolMap) : Environment()
    {
        for (auto functionPair : symbolMap)
        {
            insert(functionPair.first, functionPair.second);
        }
    }

    Environment(const std::shared_ptr<Environment<T>> &outerEnvironment)
        : outerEnvironment(outerEnvironment),
          symbolTable()
    {
    }

    void insert(const std::string &identifier, const Value<T> &symbol)
    {
        symbolTable[identifier] = symbol;
    }

    Value<T> lookup(const std::string &identifier)
    {
        if (symbolTable.count(identifier) > 0)
        {
            return symbolTable.at(identifier);
        }
        else
        {
            if (outerEnvironment != nullptr)
            {
                // Recursive lookup in the symbol table
                return outerEnvironment->lookup(identifier);
            }
            else
            {
                throw std::runtime_error(identifier + " was not definied");
            }
        }
    }

private:
    const std::shared_ptr<Environment<T>> outerEnvironment;
    std::map<std::string, Value<T>> symbolTable;
};

#endif