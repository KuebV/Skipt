//
// Created by bobwt on 2/14/2023.
//

#include "Parser.h"
#include "Expression.h"
#include "Tokenizer.h"

Parser::Parser(char* line) : tokens(new Tokenizer(line)) {}

Parser::~Parser() {
    delete tokens;
}

Expression* Parser::statement() {
    Expression *expression = nullptr;
    if ((expression=assignment()) || (expression=calculation())) ;
    return expression;
}

Expression* Parser::assignment()
{
    int tknMark = tokens->mark();
    VariableExpression *variableExpression = nullptr;
    Expression *rhsExpression = nullptr;
    if ((variableExpression=tokens->variable()) && tokens->character('=') && (rhsExpression=sum()) && tokens->atEnd())
        return new AssignmentExpression(variableExpression, rhsExpression);
    if (variableExpression)
        delete variableExpression;
    if (rhsExpression)
        delete rhsExpression;
    tokens->reset(tknMark);
    return nullptr;
}

Expression* Parser::calculation()
{
    Expression *result;
    if ((result=sum()) && tokens->atEnd())
        return result;
    if(result) {
        delete result;
        result == nullptr;
    }
    return nullptr;
}

Expression* Parser::sum()
{
    int mark = tokens->mark();
    Expression *lhs = product();
    Expression *rhs;

    while (lhs) {
        if (tokens->character('+')) {
            if (rhs=product())
                lhs = new ArithmeticExpression('+', lhs, rhs);
            else {
                delete lhs;
                lhs = nullptr;
            }
        }
        else if (tokens->character('-')) {
            rhs = product();
            if (rhs)
                lhs = new ArithmeticExpression('-', lhs, rhs);
            else {
                delete lhs;
                lhs = nullptr;
            }
        }
        else
            break;
    }

    if (lhs == nullptr)
        tokens->reset(mark);
    return lhs;
}

Expression* Parser::product()
{
    int mark = tokens->mark();
    Expression *lhs = factor();
    Expression *rhs;
    while (lhs) {
        if (tokens->character('*')) {
            rhs = factor();
            if (rhs)
                lhs = new ArithmeticExpression('*', lhs, rhs);
            else {
                delete lhs;
                lhs = nullptr;
            }
        }
        else if (tokens->character('/')) {
            if (rhs=factor())
                lhs = new ArithmeticExpression('/', lhs, rhs);
            else {
                delete lhs;
                lhs = nullptr;
            }
        }
        else if (tokens->character('%')){
            if (rhs=factor()){
                lhs = new ArithmeticExpression('%', lhs, rhs);
            }
            else{
                delete lhs;
                lhs = nullptr;
            }
        }
        else
            break;
    }
    if (lhs == nullptr)
        tokens->reset(mark);
    return lhs;
}

Expression* Parser::factor() {
    Expression *expression = nullptr;
    if ((expression=power()) || (expression=term()))
        ;
    return expression;
}

Expression* Parser::power()
{
    int powerMarker = tokens->mark();
    Expression *leftHand = nullptr;
    Expression *rightHand = nullptr;

    if ((leftHand=term()) && tokens->character('^') && (rightHand=factor()))
        return new ArithmeticExpression('^', leftHand, rightHand);

    if (leftHand)
        delete leftHand;

    tokens->reset(powerMarker);
    return nullptr;
}

Expression* Parser::term()
{
    Expression *expression = nullptr;
    if ((expression=group()) || (expression=tokens->variable()) || (expression=tokens->number()))
        ;
    return expression;
}

Expression* Parser::group() {
    int mark = tokens->mark();
    Expression *exp = nullptr;
    if (tokens->character('(') && (exp=sum()) && (tokens->character(')')))
        ;
    else if (exp) {
        delete exp;
        exp = nullptr;
        tokens->reset(mark);
    }
    return exp;
}
