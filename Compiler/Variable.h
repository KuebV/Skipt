//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_VARIABLE_H
#define SKIPT_VARIABLE_H


#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include "../Extensions/StringExt.h"

class Variable{
public:
    enum dataTypes{
        t_integer = 0,
        t_string = 1,
        t_double = 2,
        t_float,
        t_char,
        t_intArray,
        t_strArray,
        t_doubleArray,
        t_floatArray,
        t_charArray,
        t_reference,
        t_unknownArrayType,
        t_empty,
        t_boolean
    };

    dataTypes dataType;
    std::string name;
    std::string value;

    static void DefineVariable(std::string _name, std::string value, dataTypes varType, bool isConditional){
        Variable newToken; newToken.name = _name; newToken.value = value; newToken.dataType = varType;
        if (isConditional){
            ConditionalVariableMap.insert({_name, newToken});
        }
        else
            variableMap.insert({_name, newToken});
    }

    static void DefineVariable(Variable token){
        variableMap.insert({token.name, token});
    }

    static void recastVariable(std::string name, dataTypes newType, std::string value){
        Variable token; token.name = name; token.value = value; token.dataType = newType;
        variableMap.find(name)->second = token;
    }

    static void modifyVariable(Variable token, std::string value){
        Variable newToken = token; newToken.value = value;
        variableMap.find(token.name)->second = newToken;
    }

    static Variable getVariable(std::string name){
        return variableMap[name];
    }

    static Variable getTemporaryVariable(std::string name){
        return ConditionalVariableMap[name];
    }

    static Variable getAllVariables(std::string name){ // Really helpful for conditional statements in which we are allowed to look into both tokens
        if (variableMap.find(name) != variableMap.end())
            return variableMap[name];

        if (ConditionalVariableMap.find(name) != variableMap.end())
            return ConditionalVariableMap[name];

        return {};
    }

    static bool variableExists(std::string name){
        if (variableMap.find(name) == variableMap.end())
            return false;
        return true;
    }

    static bool ConvertToInt(std::string str){
        return std::stoi(str);
    }

    static bool ConvertToInt(double doub){
        return int(doub);
    }

    static bool ValidateType(std::string value, dataTypes expcectedType){
        switch (expcectedType){
            case t_integer:
                if (StringExt::IsInteger(value)){
                    return true;
                }
                return false;
            case t_double:
                if (StringExt::IsDouble(value)){
                    return true;
                }
                return false;
        }
    }

    static dataTypes GetPrimitiveType(dataTypes advancedType){
        switch (advancedType){
            case t_intArray:
                return t_integer;
            case t_doubleArray:
                return t_double;
            default:
                return advancedType;
        }
    }

    static bool DeleteToken(std::string tokenName){
        if (variableMap.find(tokenName) == variableMap.end())
            return false;

        variableMap.erase(tokenName);
        return true;
    }

    static void CleanTokens(std::vector<std::string> &tokens){
        for (int i = 0; i < tokens.size(); i++){
            tokens[i] = StringExt::Strip(tokens[i]);
        }
    }

    static void ConvertToTokenValues(std::vector<std::string> &tokens){
        if (tokens.size() == 1){
            tokens[0] = Variable::getVariable(tokens[0]).value;
        }
        else{
            for (int i = 0; i < tokens.size(); i++){
                if (Variable::variableExists(tokens[i])){
                    tokens[i] = Variable::getVariable(tokens[i]).value;
                }
            }
        }
    }

    static void ConvertToTokenValue(std::string& token){
        if (Variable::variableExists(token)){
            token = Variable::getVariable(token).value;
        }
    }

    static bool IsArray(Variable token){
        if (token.dataType == t_intArray || token.dataType == t_doubleArray || token.dataType == t_strArray || token.dataType == t_floatArray){
            return true;
        }
        return false;
    }

    static std::map<std::string, Variable> variableMap;
    static std::map<std::string, Variable> ConditionalVariableMap;
};


#endif //SKIPT_VARIABLE_H
