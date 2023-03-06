//
// Created by bobwt on 2/28/2023.
//

#include "Functions.h"
#include "SystemFunction.h"
#include "../ExpressionParser.h"
#include "StringFunction.h"
#include "InternalFunction.h"
#include "MathFunction.h"

enum ClassFunctions{
    System,
    Math,
    String,
    Array,
    Internal,
    Null
};

ClassFunctions HashString(std::string const& str){
    if (str == "System") return System;
    else if (str == "Math") return Math;
    else if (str == "String") return String;
    else if (str == "Array") return Array;
    else if (str == "Internal") return Internal;
    else return Null;
}

// This is modified from ExpressionParser.cpp, for functions, we need to parse by commas, not by spaces
std::string ModifiedReplaceVariableNames(std::string line){
    std::string temp = line;
    std::vector<std::string> values = String::Split(line, ",");

    // Serialize them in case they contain any spaces
    for (int i = 0; i < values.size(); i++){
        values[i] = String::Strip(values[i]);

        if (Token::tokenExists(values[i])){
            temp = String::Replace(temp, values[i] ,Token::getToken(values[i]).value);
        }
    }
    return temp;
}

Token Functions::HandleCallFunction(std::string functionCall) {

    size_t x = functionCall.find('(');
    std::string functionName = functionCall.substr(0, x);

    std::vector<std::string> functionElements = String::Split(functionName, "::");
    std::string classFunction = functionElements[0];
    std::string methodFunction = functionElements[1];

    std::string arguments = String::Substring(functionCall, "(", ")");
    arguments = ModifiedReplaceVariableNames(arguments);

    switch (HashString(classFunction)){
        case Null:{
            std::cout << "\n[Error] | [Functions.cpp] [HandleCallFunction]: No class function found!!\n";
            std::cout << "        |> " << functionCall << "\n";
            return {};
        }
        case System:{
            return SystemFunction::HandleCall(methodFunction, arguments);
        }
        case Math:
            return MathFunction::HandleCall(methodFunction, arguments);
        case String:
            return StringFunction::HandleCall(methodFunction, arguments);
        case Array:
            break;
        case Internal:
            return InternalFunction::HandleCall(methodFunction,arguments);
            break;
    }

    return {};
}
