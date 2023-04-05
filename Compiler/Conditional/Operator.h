//
// Created by bobwt on 2/16/2023.
//

#ifndef SKIPT_OPERATOR_H
#define SKIPT_OPERATOR_H

#include <string>
#include "../Variable.h"
#include "../../Extensions/StringExt.h"
#include "../../Extensions/Array.h"


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


    };

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

        return NotEqualTo;
    }

    // There has to be a better way of doing this?
    static bool Condition(Variable t_one, Variable t_two, Operators op){

        if (t_one.dataType != Variable::t_boolean || t_two.dataType != Variable::t_boolean){
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
                    switch (t_two.dataType){
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
        else if (t_one.dataType == Variable::t_boolean || t_two.dataType == Variable::t_boolean){
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

    static std::string ResultOfOperator(Variable tempTokenOne, Variable tempTokenTwo, Operators op, Variable::dataTypes expectingType){
        std::string tempString = "";
        switch (op){
            case Operators::Modulo:
                tempString = std::to_string(StringExt::ToInteger(tempTokenOne.value) % StringExt::ToInteger(tempTokenTwo.value));
                return tempString;
            case Operators::Increment:
                break;
        }

        return "";
    }
};


#endif //SKIPT_OPERATOR_H
