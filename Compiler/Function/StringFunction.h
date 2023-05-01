//
// Created by bobwt on 3/2/2023.
//

#ifndef SKIPT_STRINGFUNCTION_H
#define SKIPT_STRINGFUNCTION_H

#include <string>
#include <unordered_map>
#include "../Variable.h"

class StringFunction {
public:
    enum MethodFunctions{
        Format,
        Replace,
        Substring,
        At,
        Find,
        Contains,
        Null,
        Length

    };

    static Variable HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_STRINGFUNCTION_H
