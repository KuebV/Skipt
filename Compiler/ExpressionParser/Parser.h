//
// Created by bobwt on 2/14/2023.
//

#ifndef SKIPT_PARSER_H
#define SKIPT_PARSER_H

class Expression;
class Tokenizer;

class Parser
{
public:
    Parser(char* line);
    ~Parser();

    Expression* statement();
private:
    Tokenizer* tokens;

    Expression* assignment();
    Expression* calculation();
    Expression* sum();
    Expression* product();
    Expression* factor();
    Expression* power();
    Expression* term();
    Expression* group();
};


#endif //SKIPT_PARSER_H
