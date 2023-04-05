//
// Created by bobwt on 2/28/2023.
//

#ifndef SKIPT_SYSTEMFUNCTION_H
#define SKIPT_SYSTEMFUNCTION_H


#include "../Variable.h"


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
        RandInt,
        InitRand,
        RandArray,
        Null,
        Out
    };

    static Variable HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_SYSTEMFUNCTION_H
