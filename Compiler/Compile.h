//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_COMPILE_H
#define SKIPT_COMPILE_H


#include <vector>
#include "Token.h"

class Compile {
public:
    void Run(std::string fileName, bool asExternal);
    std::string parseString(std::string line, int lineNumber);
    bool sameDataType(Token token, std::string value, int lineNumber);

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
