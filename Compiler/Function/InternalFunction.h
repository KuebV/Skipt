//
// Created by bobwt on 3/4/2023.
//

#ifndef SKIPT_INTERNALFUNCTION_H
#define SKIPT_INTERNALFUNCTION_H


#include "../Token.h"

class InternalFunction {
public:
    enum MethodFunctions{
        ListTokens,
        RunSkiptFile,
        Goto,
        Null
    };

    static Token HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_INTERNALFUNCTION_H
