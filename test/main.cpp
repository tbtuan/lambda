#include "../src/evaluator.h"
#include <iostream>
#include <vector>
#include <limits>
#include "unittest.h"
#include "testLexer.cpp"
#include "testParser.cpp"
#include "testSemanticanalyzer.cpp"
#include "testInterpreter.cpp"

int main()
{
    testLexer();
    testParser();
    testSemanticanalyzer();
    testInterpreter();
}