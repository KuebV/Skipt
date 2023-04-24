//
// Created by bobwt on 2/16/2023.
//

#ifndef SKIPT_OPERATOR_H
#define SKIPT_OPERATOR_H

#include <string>
#include "../Variable.h"
#include "../../Extensions/StringExt.h"
#include "../../Extensions/Array.h"
#include "../Function/ArrayFunctions.h"
#include "../../ModifiedType/String.h"
#include "../ExitMessage.h"


class Operator {
public:

    enum Operators{
        GreaterThan,
        GreaterEqualThan,
        EqualTo,
        LessThan,
        LessEqualThan,
        NotEqualTo,
        Modulo,
        SingleIncrement,
        Increment,
        SingleDecrement,
        Decrement,
        In,
        Null,
        Multiplication,
        Division


    };

    static bool IsOperator(char currentCharacter, char nextCharacter){
        std::string op; op += currentCharacter; op += nextCharacter;
        if (ParseOperator(op) != Null)
            return true;
        return false;

    }

    static Operators ParseOperator(std::string str){
        if (str == ">")
            return GreaterThan;

        if (str == ">=")
            return GreaterEqualThan;

        if (str == "==")
            return EqualTo;

        if (str == "<")
            return LessThan;

        if (str == "<=")
            return LessEqualThan;

        if (str == "!=")
            return NotEqualTo;

        if (str == "%")
            return Modulo;

        if (str == "++")
            return SingleIncrement;

        if (str == "+=")
            return Increment;

        if (str == "--")
            return SingleDecrement;

        if (str == "-=")
            return Decrement;

        if (str == "in")
            return In;

        return Null;
    }

    // There has to be a better way of doing this?
    static bool Condition(Variable t_one, Variable t_two, Operators op){

        if (t_one.type != Variable::t_boolean || t_two.type != Variable::t_boolean){
            double var1, var2;
            var1 = StringExt::ToDouble(t_one.value);
            var2 = StringExt::ToDouble(t_two.value);

            switch (op){
                case Operators::GreaterThan:
                    if (var1 > var2)
                        return true;
                    return false;
                case Operators::GreaterEqualThan:
                    if (var1 >= var2)
                        return true;
                    return false;
                case Operators::EqualTo:
                    if (var1 == var2)
                        return true;
                    return false;
                case Operators::LessThan:
                    if (var1 < var2)
                        return true;
                    return false;
                case Operators::LessEqualThan:
                    if (var1 <= var2)
                        return true;
                    return false;
                case Operators::NotEqualTo:
                    if (var1 != var2)
                        return true;
                    return false;
                case Operators::In:{
                    switch (t_two.type){
                        case Variable::t_intArray:
                            if (Array::Contains(StringExt::ToIntVector(t_two.value), StringExt::ToInteger(t_one.value)))
                                return true;
                            return false;
                        case Variable::t_doubleArray:
                            if (Array::Contains(StringExt::ToDoubleVector(t_two.value), StringExt::ToDouble(t_one.value)))
                                return true;
                            return false;
                    }
                }
            }
        }
        else if (t_one.type == Variable::t_boolean || t_two.type == Variable::t_boolean){
            bool var1, var2;
            var1 = StringExt::ToBoolean(t_one.value);
            var2 = StringExt::ToBoolean(t_two.value);

            switch (op){
                case Operators::EqualTo:{
                    if (var1 == var2)
                        return true;
                    return false;
                }
                case Operators::NotEqualTo:{
                    if (var1 != var2)
                        return true;
                    return false;
                }
            }
        }
    }


    static void ExecuteOperator(Operators _operator, std::string line){
        ExitMessage exitMsg("Operator.h");
        String lineString(line);
        switch (_operator){
            case Operators::Increment:{
                size_t indexAdd = lineString.Until('+');
                String variableInitial(lineString.Substring(0, indexAdd));
                variableInitial.ptr_Strip(String::StripOptions::All);

                if (!Variable::Exists(variableInitial.ToString())){
                    exitMsg.Error("ExecuteOperator", "Variable does not exist!", line, 1);
                }

                size_t indexEquals = lineString.Until("+=");
                String extraVariable(lineString.Substring(indexEquals + 2, lineString.Length));
                extraVariable.ptr_Strip(String::StripOptions::All);

                extraVariable.ptr_Strip(String::StripOptions::All);
                if (Variable::Exists(extraVariable.ToString())){
                    extraVariable.ptr_ReplaceSingle(extraVariable.ToString(), Variable::Get(extraVariable.ToString()).value);
                }

                Variable variableToModify = Variable::Get(variableInitial.ToString());

                std::unique_ptr<bool> modify = std::make_unique<bool>();

                switch (variableToModify.type){
                    case Variable::t_string:{
                        extraVariable.ptr_ContentBetween("\"", "\"");
                        variableToModify.value += extraVariable.ToString();
                        *modify = true;
                        break;
                    }
                    case Variable::t_integer:
                    case Variable::t_double:
                    case Variable::t_float:{
                        if (!StringExt::IsDouble(extraVariable.ToString())){
                            exitMsg.Error("ExecuteOperator", extraVariable.ToString() + " is not compatible with " + variableInitial.ToString(), line, 1);
                        }

                        double extraValue = StringExt::ToDouble(extraVariable.ToString());
                        double initialValue = StringExt::ToDouble(variableToModify.value);

                        initialValue += extraValue;
                        variableToModify.value = std::to_string(initialValue);
                        *modify = true;
                        break;
                    }
                    case Variable::t_strArray:
                    case Variable::t_doubleArray:
                    case Variable::t_intArray:
                    case Variable::t_floatArray:{
                        ArrayFunctions::HandleCall("add", variableToModify.name + ", " + extraVariable.ToString());
                        *modify = false;
                        break;
                    }
                }
                if (*modify)
                    Variable::modifyVariable(variableToModify);

                break;
            }
            case Operators::Decrement:{
                size_t indexAdd = lineString.Until('-');
                String variableInitial(lineString.Substring(0, indexAdd));
                variableInitial.ptr_Strip(String::StripOptions::All);

                if (!Variable::Exists(variableInitial.ToString())){
                    exitMsg.Error("ExecuteOperator", "Variable does not exist!", line, 1);
                }

                size_t indexEquals = lineString.Until("-=");
                String extraVariable(lineString.Substring(indexEquals + 2, lineString.Length));
                extraVariable.ptr_Strip(String::StripOptions::All);

                if (Variable::Exists(extraVariable.ToString())){
                    extraVariable.ptr_ReplaceSingle(extraVariable.ToString(), Variable::Get(extraVariable.ToString()).value);
                }

                Variable variableToModify = Variable::Get(variableInitial.ToString());

                std::unique_ptr<bool> modify = std::make_unique<bool>();

                switch (variableToModify.type){
                    case Variable::t_string:{
                        exitMsg.Error("ExecuteOperator", "Cannot decrement a string!", line, 1);
                        break;
                    }
                    case Variable::t_integer:
                    case Variable::t_double:
                    case Variable::t_float:{
                        if (!StringExt::IsDouble(extraVariable.ToString())){
                            exitMsg.Error("ExecuteOperator", extraVariable.ToString() + " is not compatible with " + variableInitial.ToString(), line, 1);
                        }

                        double extraValue = StringExt::ToDouble(extraVariable.ToString());
                        double initialValue = StringExt::ToDouble(variableToModify.value);

                        initialValue += extraValue;
                        variableToModify.value = std::to_string(initialValue);
                        *modify = true;
                        break;
                    }
                    case Variable::t_strArray:
                    case Variable::t_doubleArray:
                    case Variable::t_intArray:
                    case Variable::t_floatArray:{
                        exitMsg.Error("ExecuteOperator", "Cannot decrement an array!", line, 1);
                        break;
                    }
                }
                if (*modify)
                    Variable::modifyVariable(variableToModify);

                break;
            }
            case Operators::Multiplication:{
                size_t indexAdd = lineString.Until('*');
                String variableInitial(lineString.Substring(0, indexAdd));
                variableInitial.ptr_Strip(String::StripOptions::All);

                if (!Variable::Exists(variableInitial.ToString())){
                    exitMsg.Error("ExecuteOperator", "Variable does not exist!", line, 1);
                }

                size_t indexEquals = lineString.Until("*=");
                String extraVariable(lineString.Substring(indexEquals + 2, lineString.Length));
                extraVariable.ptr_Strip(String::StripOptions::All);

                if (Variable::Exists(extraVariable.ToString())){
                    extraVariable.ptr_ReplaceSingle(extraVariable.ToString(), Variable::Get(extraVariable.ToString()).value);
                }

                Variable variableToModify = Variable::Get(variableInitial.ToString());

                std::unique_ptr<bool> modify = std::make_unique<bool>();

                switch (variableToModify.type){
                    case Variable::t_string:{
                        exitMsg.Error("ExecuteOperator", "Cannot multiply a string!", line, 1);
                        break;
                    }
                    case Variable::t_integer:
                    case Variable::t_double:
                    case Variable::t_float:{
                        if (!StringExt::IsDouble(extraVariable.ToString())){
                            exitMsg.Error("ExecuteOperator", extraVariable.ToString() + " is not compatible with " + variableInitial.ToString(), line, 1);
                        }

                        double extraValue = StringExt::ToDouble(extraVariable.ToString());
                        double initialValue = StringExt::ToDouble(variableToModify.value);

                        initialValue *= extraValue;
                        variableToModify.value = std::to_string(initialValue);
                        *modify = true;
                        break;
                    }
                    case Variable::t_strArray:
                    case Variable::t_doubleArray:
                    case Variable::t_intArray:
                    case Variable::t_floatArray:{
                        exitMsg.Error("ExecuteOperator", "Cannot multiply an array!", line, 1);
                        break;
                    }
                }
                if (*modify)
                    Variable::modifyVariable(variableToModify);

                break;
            }
            case Operators::Division:{
                size_t indexAdd = lineString.Until('/');
                String variableInitial(lineString.Substring(0, indexAdd));
                variableInitial.ptr_Strip(String::StripOptions::All);

                if (!Variable::Exists(variableInitial.ToString())){
                    exitMsg.Error("ExecuteOperator", "Variable does not exist!", line, 1);
                }

                size_t indexEquals = lineString.Until("/=");
                String extraVariable(lineString.Substring(indexEquals + 2, lineString.Length));
                extraVariable.ptr_Strip(String::StripOptions::All);

                if (Variable::Exists(extraVariable.ToString())){
                    extraVariable.ptr_ReplaceSingle(extraVariable.ToString(), Variable::Get(extraVariable.ToString()).value);
                }

                Variable variableToModify = Variable::Get(variableInitial.ToString());

                std::unique_ptr<bool> modify = std::make_unique<bool>();

                switch (variableToModify.type){
                    case Variable::t_string:{
                        exitMsg.Error("ExecuteOperator", "Cannot divide a string!", line, 1);
                        break;
                    }
                    case Variable::t_integer:
                    case Variable::t_double:
                    case Variable::t_float:{
                        if (!StringExt::IsDouble(extraVariable.ToString())){
                            exitMsg.Error("ExecuteOperator", extraVariable.ToString() + " is not compatible with " + variableInitial.ToString(), line, 1);
                        }

                        double extraValue = StringExt::ToDouble(extraVariable.ToString());
                        double initialValue = StringExt::ToDouble(variableToModify.value);

                        initialValue /= extraValue;
                        variableToModify.value = std::to_string(initialValue);
                        *modify = true;
                        break;
                    }
                    case Variable::t_strArray:
                    case Variable::t_doubleArray:
                    case Variable::t_intArray:
                    case Variable::t_floatArray:{
                        exitMsg.Error("ExecuteOperator", "Cannot divide an array!", line, 1);
                        break;
                    }
                }
                if (*modify)
                    Variable::modifyVariable(variableToModify);

                break;
            }
            case Operators::Modulo:{
                size_t indexAdd = lineString.Until('%');
                String variableInitial(lineString.Substring(0, indexAdd));
                variableInitial.ptr_Strip(String::StripOptions::All);

                if (!Variable::Exists(variableInitial.ToString())){
                    exitMsg.Error("ExecuteOperator", "Variable does not exist!", line, 1);
                }

                size_t indexEquals = lineString.Until("%=");
                String extraVariable(lineString.Substring(indexEquals + 2, lineString.Length));
                extraVariable.ptr_Strip(String::StripOptions::All);

                if (Variable::Exists(extraVariable.ToString())){
                    extraVariable.ptr_ReplaceSingle(extraVariable.ToString(), Variable::Get(extraVariable.ToString()).value);
                }

                Variable variableToModify = Variable::Get(variableInitial.ToString());

                std::unique_ptr<bool> modify = std::make_unique<bool>();

                switch (variableToModify.type){
                    case Variable::t_string:{
                        exitMsg.Error("ExecuteOperator", "Cannot find remainder of a string!", line, 1);
                        break;
                    }
                    case Variable::t_integer:
                    case Variable::t_double:
                    case Variable::t_float:{
                        if (!StringExt::IsDouble(extraVariable.ToString())){
                            exitMsg.Error("ExecuteOperator", extraVariable.ToString() + " is not compatible with " + variableInitial.ToString(), line, 1);
                        }

                        int extraValue = StringExt::ToInteger(extraVariable.ToString());
                        int initialValue = StringExt::ToInteger(variableToModify.value);

                        initialValue = initialValue % extraValue;
                        variableToModify.value = std::to_string(initialValue);
                        *modify = true;
                        break;
                    }
                    case Variable::t_strArray:
                    case Variable::t_doubleArray:
                    case Variable::t_intArray:
                    case Variable::t_floatArray:{
                        exitMsg.Error("ExecuteOperator", "Cannot find remainder of an array!", line, 1);
                        break;
                    }
                }
                if (*modify)
                    Variable::modifyVariable(variableToModify);

                break;
            }
        }
    }
};


#endif //SKIPT_OPERATOR_H
