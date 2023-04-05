//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_COMPILE_H
#define SKIPT_COMPILE_H


#include "Compile.h"
#include "FileReader.h"
#include "../Extensions/StringExt.h"
#include "ExpressionParser/Parser.h"
#include "ExpressionParser.h"
#include "../Compiler/ExpressionParser/Expression.h"
#include "Conditional/Operator.h"
#include "Function/Functions.h"
#include "../Property/PropertyReference.h"
#include "../Property/PropertyFile.h"
#include "ExitMessage.h"
#include <string>
#include <cstring>
#include <unordered_map>
#include <chrono>
#include "Variable.h"

class Compile {
public:
    void Run(std::string fileName, bool asExternal);
    std::string parseString(std::string line, int lineNumber);
    bool sameDataType(Variable token, std::string value, int lineNumber);

    enum VariableTypes{
        Integer,
        String,
        Double,
        Float,
        Boolean,
        Reference,
        IntegerArray,
        StringArray,
        DoubleArray,
        FloatArray,
        BooleanArray,
        Void,
        Free,
        UnsafeFree,
        If,
        While

    };
};


#endif //SKIPT_COMPILE_H
