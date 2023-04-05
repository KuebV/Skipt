//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_EXPRESSIONPARSER_H
#define SKIPT_EXPRESSIONPARSER_H


#include "Variable.h"

class ExpressionParser {
public:
    static std::string ReplaceVariableNames(std::string line);
};


#endif //SKIPT_EXPRESSIONPARSER_H
