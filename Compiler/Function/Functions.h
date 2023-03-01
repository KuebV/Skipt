//
// Created by bobwt on 2/28/2023.
//

#ifndef SKIPT_FUNCTIONS_H
#define SKIPT_FUNCTIONS_H


#include "../Token.h"

class Functions {
public:

    // The entire function call should be processed through the argument
    // var = func(x)
    static Token HandleCallFunction(std::string functionCall);
};


#endif //SKIPT_FUNCTIONS_H
