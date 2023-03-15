//
// Created by bobwt on 3/2/2023.
//

#include <iostream>
#include "StringFunction.h"
#include "../ExitMessage.h"

StringFunction::MethodFunctions GetStringFunction(std::string const& str){
    const std::unordered_map<std::string, StringFunction::MethodFunctions> functionTable{
            { "format", StringFunction::MethodFunctions::Format},
            { "replace", StringFunction::MethodFunctions::Replace},
            { "substring", StringFunction::MethodFunctions::Substring},
            { "at", StringFunction::MethodFunctions::At},
            { "find", StringFunction::MethodFunctions::Find},
            { "contains", StringFunction::MethodFunctions::Contains}
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

    ExitMessage exitMessage = ExitMessage("StringFunction.cpp");

    switch (GetStringFunction(function)){
        case Format: {
            std::vector<std::string> args = String::Split(arguments, ",");
            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

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

            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

            std::string initialString = args[0];
            initialString = String::Replace(initialString, args[1], args[2]);

            replaceToken.value = initialString;
            replaceToken.dataType = Token::t_string;

            return replaceToken;
        }
        case Substring:{
            std::vector<std::string> args = String::Split(arguments, ",");
            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

            if (String::IsInteger(args[1])){
                exitMessage.Error("HandleCall.Substring", args[1] + " is not of expected type integer", arguments, 1);
            }
            int atIndex = String::ToInteger(args[1]);

            if (String::IsInteger(args[2])){
                exitMessage.Error("HandleCall.Substring", args[2] + " is not of expected type integer", arguments, 1);
            }
            int lengthOf = String::ToInteger(args[2]);

            Token token;
            token.dataType = Token::t_string;
            token.value = args[0].substr(atIndex, lengthOf);

            return token;
        }

        case At:
            break;
        case Find:
            break;
        case Contains:{
            std::vector<std::string> args = String::Split(arguments, ",");
            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

            Token token;
            token.dataType = Token::t_boolean;
            if (String::Contains(args[0], args[1])){
                token.value = "true";
            }
            else{
                token.value = "false";
            }

            return token;
        }

    }
}
