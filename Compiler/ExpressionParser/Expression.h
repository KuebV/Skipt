//
// Created by bobwt on 2/14/2023.
//

#ifndef SKIPT_EXPRESSION_H
#define SKIPT_EXPRESSION_H


#include <map>
#include <string>
#include "unordered_map"

class Expression
{
public:
    Expression();
    virtual ~Expression();

    static double lookUp(const std::string& var);
    static void record(const std::string& var, double value);

    virtual double evaluate() = 0;
protected:
    static std::unordered_map<std::string, double> vars;
};

class NumberExpression : public Expression
{
public:
    NumberExpression(double val);

    virtual double evaluate();
private:
    double value;
};

class VariableExpression : public Expression
{
public:
    VariableExpression(const std::string& var);

    virtual double evaluate();

    std::string getVar() const { return variable; }
private:
    std::string variable;
};

class ArithmeticExpression : public Expression
{
public:
    ArithmeticExpression(char op, Expression *left, Expression *right);
    virtual ~ArithmeticExpression();

    virtual double evaluate();
private:
    Expression *left, *right;
    char op;
};

class AssignmentExpression : public Expression
{
public:
    AssignmentExpression(VariableExpression *var, Expression *value);
    virtual ~AssignmentExpression();

    virtual double evaluate();
private:
    VariableExpression *var;
    Expression *value;
};


#endif //SKIPT_EXPRESSION_H
