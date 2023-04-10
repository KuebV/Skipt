//
// Created by bobwt on 3/2/2023.
//

#include <iostream>
#include "StringFunction.h"
#include "../ExitMessage.h"
#include "../../ModifiedType/String.h"

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

Variable StringFunction::HandleCall(std::string function, std::string arguments) {
    Variable emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Variable::t_empty;

    ExitMessage exitMessage = ExitMessage("StringFunction.cpp");

    switch (GetStringFunction(function)){
        case Format: {
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            std::string initialString = args[0];
            for (int i = 1; i < args.size();i++){
                if (args.size() >= 100)
                    initialString = String(initialString).ReplaceMulti("{" + std::to_string(i - 1) + "}", args[i]).ToString();
                else
                    initialString = StringExt::Replace(initialString, "{" + std::to_string(i - 1) + "}", args[i]);
            }

            Variable formatToken;
            formatToken.value = initialString;
            formatToken.dataType = Variable::t_string;

            return formatToken;
        }
        case Replace: {
            Variable replaceToken;

            // Replace must contain existing variables in the arguments
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            // args[0] == initial string
            // args[1] == string to replace
            // args[2] == replacing string

            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            std::string initialString = args[0];
            initialString = StringExt::Replace(initialString, args[1], args[2]);

            replaceToken.value = initialString;
            replaceToken.dataType = Variable::t_string;

            return replaceToken;
        }
        case Substring:{
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            if (StringExt::IsInteger(args[1])){
                exitMessage.Error("HandleCall.Substring", args[1] + " is not of expected type integer", arguments, 1);
            }
            int atIndex = StringExt::ToInteger(args[1]);

            if (StringExt::IsInteger(args[2])){
                exitMessage.Error("HandleCall.Substring", args[2] + " is not of expected type integer", arguments, 1);
            }
            int lengthOf = StringExt::ToInteger(args[2]);

            Variable token;
            token.dataType = Variable::t_string;
            token.value = args[0].substr(atIndex, lengthOf);

            return token;
        }

        case At:
            break;
        case Find:
            break;
        case Contains:{
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            Variable token;
            token.dataType = Variable::t_boolean;
            token.value = StringExt::Contains(args[0], args[1]) ? "true" : "false";

            return token;

        }

        case Null:
            break;
    }
}
