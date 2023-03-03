//
// Created by bobwt on 3/2/2023.
//

#include <iostream>
#include "StringFunction.h"

StringFunction::MethodFunctions GetStringFunction(std::string const& str){
    const std::unordered_map<std::string, StringFunction::MethodFunctions> functionTable{
            { "format", StringFunction::MethodFunctions::Format},
            { "replace", StringFunction::MethodFunctions::Replace},
            { "substring", StringFunction::MethodFunctions::Substring},
    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return StringFunction::MethodFunctions::Null;
    }
}

Token StringFunction::HandleCall(std::string function, std::string arguments) {
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    switch (GetStringFunction(function)){
        case Format: {
            std::vector<std::string> args = String::Split(arguments, ",");
            for (int i = 0; i < args.size(); i++)
                args[i] = String::Strip(args[i]);

            std::string initialString = args[0];
            for (int i = 1; i < args.size();i++){
                initialString = String::Replace(initialString, "{" + std::to_string(i - 1) + "}", args[i]);
            }

            Token formatToken;
            formatToken.value = initialString;
            formatToken.dataType = Token::t_string;

            return formatToken;
        }
        case Replace: {
            Token replaceToken;

            // Replace must contain existing variables in the arguments
            std::vector<std::string> args = String::Split(arguments, ",");
            // args[0] == initial string
            // args[1] == string to replace
            // args[2] == replacing string

            for (int i = 0; i < args.size(); i++){
                args[i] = String::Strip(args[i]);
            }

            std::string initialString = args[0];
            initialString = String::Replace(initialString, args[1], args[2]);

            replaceToken.value = initialString;
            replaceToken.dataType = Token::t_string;

            return replaceToken;
        }
        case Substring:
            break;
        case Null:
            break;
    }
}
