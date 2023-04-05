//
// Created by bobwt on 2/28/2023.
//

#ifndef SKIPT_FUNCTIONS_H
#define SKIPT_FUNCTIONS_H


#include "../Variable.h"

class Functions {
public:

    // The entire function call should be processed through the argument
    // ref class:method() >> var
    static Variable HandleCallFunction(std::string functionCall);
};


#endif //SKIPT_FUNCTIONS_H
