//
// Created by mary on 3/6/2023.
//

#ifndef SKIPT_MATHFUNCTION_H
#define SKIPT_MATHFUNCTION_H


#include "../Variable.h"

class MathFunction {
public:
    enum MethodFunctions{
        Null,
        Modulo,
        IsPrime,
    };

    static Variable HandleCall(std::string function, std::string arguments);

};


#endif //SKIPT_MATHFUNCTION_H
