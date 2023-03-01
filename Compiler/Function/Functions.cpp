//
// Created by bobwt on 2/28/2023.
//

#include "Functions.h"
#include "SystemFunction.h"
#include "../ExpressionParser.h"

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

Token Functions::HandleCallFunction(std::string functionCall) {
    size_t x = functionCall.find('(');
    std::string functionName = functionCall.substr(0, x);
    std::string classFunction = String::Split(functionName, "::")[0];
    std::string methodFunction = String::Split(functionName, "::")[1];

    std::string arguments = String::Substring(functionCall, "(", ")");
    arguments = ExpressionParser::ReplaceVariableNames(arguments);

    switch (HashString(classFunction)){
        case Null:{
            std::cout << "There is no class function with the name of " << classFunction << "\n";
            return {};
        }
        case System:{
            return SystemFunction::HandleCall(methodFunction, arguments);
        }
        case Math:
            break;
        case String:
            break;
        case Array:
            break;
        case Internal:
            break;
    }

    return {};
}
