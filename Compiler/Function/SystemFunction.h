//
// Created by bobwt on 2/28/2023.
//

#ifndef SKIPT_SYSTEMFUNCTION_H
#define SKIPT_SYSTEMFUNCTION_H


#include "../Token.h"


class SystemFunction {
public:
    enum MethodFunctions{
        Exit,
        FileCreate,
        FileDelete,
        FileWrite,
        FileRead,
        FileAppend,
        FileExists,
        Null,
        Out
    };

    static Token HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_SYSTEMFUNCTION_H
