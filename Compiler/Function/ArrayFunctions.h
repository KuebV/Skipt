//
// Created by Rob on 3/28/2023.
//

#ifndef SKIPT_ARRAYFUNCTIONS_H
#define SKIPT_ARRAYFUNCTIONS_H


#include "../Variable.h"

class ArrayFunctions {
public:
    enum MethodFunctions{
        At,
        Add,
        Remove,
        Length,
        Sort,
        Null
    };
    static Variable HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_ARRAYFUNCTIONS_H
