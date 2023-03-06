//
// Created by mary on 3/6/2023.
//

#ifndef SKIPT_MATHFUNCTION_H
#define SKIPT_MATHFUNCTION_H


#include "../Token.h"

class MathFunction {
public:
    enum MethodFunctions{
        Null,
        Modulo,
        IsPrime,
    };

    static Token HandleCall(std::string function, std::string arguments);

};


#endif //SKIPT_MATHFUNCTION_H
