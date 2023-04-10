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

    Variable(){
        dataType = t_empty;
        name = "undefinedVariable";
        value = "0.0";
    }

    Variable(std::string name, std::string value, dataTypes type){
        if (Variable::Exists(name)){

        }
    }

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

    void modifyVariable(Variable newVariable){
        name = newVariable.name;
        value = newVariable.value;
        dataType = newVariable.dataType;
    }

    static Variable Get(std::string name){
        if (variableMap.find(name) != variableMap.end())
            return variableMap[name];

        if (ConditionalVariableMap.find(name) != variableMap.end())
            return ConditionalVariableMap[name];

        return {};
    }


    static std::vector<std::string> GetVariableNames(){
        std::unique_ptr<std::vector<std::string>> vector = std::make_unique<std::vector<std::string>>();
        for (auto it = variableMap.begin(); it != variableMap.end(); it++){
            vector->push_back(it->first);
        }
        return *vector;
    }

    static bool Exists(std::string name){
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
            tokens[0] = Variable::Get(tokens[0]).value;
        }
        else{
            for (int i = 0; i < tokens.size(); i++){
                if (Variable::Exists(tokens[i])){
                    tokens[i] = Variable::Get(tokens[i]).value;
                }
            }
        }
    }

    static void ConvertToTokenValue(std::string& token){
        if (Variable::Exists(token)){
            token = Variable::Get(token).value;
        }
    }

    static bool IsArray(Variable token){
        if (token.dataType == t_intArray || token.dataType == t_doubleArray || token.dataType == t_strArray || token.dataType == t_floatArray){
            return true;
        }
        return false;
    }

    static bool IsArray(dataTypes type){
        if (type == t_intArray || type == t_doubleArray || type == t_strArray || type == t_floatArray){
            return true;
        }
        return false;
    }


    static std::string EnumToString(int enumValue){
        switch (enumValue){
            case 0:
                return "int";
            case 1:
                return "string";
            case 2:
                return "double";
            case 3:
                return "float";
            case 4:
                return "char";
            case 5:
                return "int[]";
            case 6:
                return "string[]";
            case 7:
                return "double[]";
            case 8:
                return "float[]";
            case 9:
                return "char[]";
            case 10:
                return "reference";
            case 11:
                return "unknown[]";
            case 12:
                return "empty";
            case 13:
                return "boolean";
        }
    }

    static std::map<std::string, Variable> variableMap;
    static std::map<std::string, Variable> ConditionalVariableMap;
};


#endif //SKIPT_VARIABLE_H
