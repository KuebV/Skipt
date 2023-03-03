//
// Created by bobwt on 3/2/2023.
//

#ifndef SKIPT_STRINGFUNCTION_H
#define SKIPT_STRINGFUNCTION_H

#include <string>
#include <unordered_map>
#include "../Token.h"

class StringFunction {
public:
    enum MethodFunctions{
        Format,
        Replace,
        Substring,
        Null

    };

    static Token HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_STRINGFUNCTION_H
