//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_TOKEN_H
#define SKIPT_TOKEN_H


#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include "../Extensions/String.h"

class Token{
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
        Token newToken; newToken.name = _name; newToken.value = value; newToken.dataType = varType;
        if (isConditional){
            ConditionalTokenMap.insert({_name, newToken});
        }
        else
            tokenMap.insert({_name, newToken});
    }

    static void DefineVariable(Token token){
        tokenMap.insert({token.name, token});
    }

    static void recastVariable(std::string name, dataTypes newType, std::string value){
        Token token; token.name = name; token.value = value; token.dataType = newType;
        tokenMap.find(name)->second = token;
    }

    static void modifyToken(Token token, std::string value){
        Token newToken = token; newToken.value = value;
        tokenMap.find(token.name)->second = newToken;
    }

    static Token getToken(std::string name){
        return tokenMap[name];
    }

    static Token getTemporaryToken(std::string name){
        return ConditionalTokenMap[name];
    }

    static Token getAllTokens(std::string name){ // Really helpful for conditional statements in which we are allowed to look into both tokens
        if (tokenMap.find(name) != tokenMap.end())
            return tokenMap[name];

        if (ConditionalTokenMap.find(name) != tokenMap.end())
            return ConditionalTokenMap[name];

        return {};
    }

    static bool tokenExists(std::string name){
        if (tokenMap.find(name) == tokenMap.end())
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
                if (String::IsInteger(value)){
                    return true;
                }
                return false;
            case t_double:
                if (String::IsDouble(value)){
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
        if (tokenMap.find(tokenName) == tokenMap.end())
            return false;

        tokenMap.erase(tokenName);
        return true;
    }

    static void CleanTokens(std::vector<std::string> &tokens){
        for (int i = 0; i < tokens.size(); i++){
            tokens[i] = String::Strip(tokens[i]);
        }
    }

    static void ConvertToTokenValues(std::vector<std::string> &tokens){
        for (int i = 0; i < tokens.size(); i++){
            if (Token::tokenExists(tokens[i])){
                tokens[i] = Token::getToken(tokens[i]).value;
            }
        }
    }

    static void ConvertToTokenValue(std::string& token){
        if (Token::tokenExists(token)){
            token = Token::getToken(token).value;
        }
    }

    static bool IsArray(Token token){
        if (token.dataType == t_intArray || token.dataType == t_doubleArray || token.dataType == t_strArray || token.dataType == t_floatArray){
            return true;
        }
        return false;
    }

    static std::map<std::string, Token> tokenMap;
    static std::map<std::string, Token> ConditionalTokenMap;
};


#endif //SKIPT_TOKEN_H
