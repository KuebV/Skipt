//
// Created by bobwt on 2/14/2023.
//

#ifndef SKIPT_TOKENIZER_H
#define SKIPT_TOKENIZER_H

#include <sstream>

class NumberExpression;
class VariableExpression;

class Tokenizer
{
public:
    Tokenizer(char* data);

    NumberExpression* number();
    VariableExpression* variable();
    bool character(char expected);
    bool atEnd();

    int mark();
    void reset(int mark);
private:
    std::stringstream str;

    void skipWhiteSpace();
};



#endif //SKIPT_TOKENIZER_H
