//
// Created by Rob on 3/28/2023.
//

#include <unordered_map>
#include <algorithm>
#include "ArrayFunctions.h"
#include "InternalFunction.h"
#include "../ExitMessage.h"
#include "../../Property/PropertyFile.h"

ArrayFunctions::MethodFunctions GetArrayFunction(std::string const& str){
    const std::unordered_map<std::string, ArrayFunctions::MethodFunctions> functionTable{
            { "at", ArrayFunctions::MethodFunctions::At},
            { "add", ArrayFunctions::MethodFunctions::Add},
            { "length", ArrayFunctions::MethodFunctions::Length}
    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return ArrayFunctions::MethodFunctions::Null;
    }
}


Token ArrayFunctions::HandleCall(std::string function, std::string arguments) {
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    ExitMessage exitMsg = ExitMessage("ArrayFunctions.cpp");

    switch (GetArrayFunction(function)){
        case At:{
            // Arguments
            // Array, Index
            std::vector<std::string> args = String::Split(arguments, ",");
            Token::CleanTokens(args);

            if (!Token::tokenExists(args[0])){
                exitMsg.Error("HandleCall.At", args[0] + " is not a defined array variable!", arguments, 1);
            }
            std::string preArray = args[0];

            Token::ConvertToTokenValues(args);

            if (!String::IsInteger(args[1])){
                exitMsg.Error("HandleCall.At", args[1] + " is expected to be of type integer!", arguments, 1);
            }
            int items = std::count(args[0].begin(),  args[0].end(), ',');

            if (String::ToInteger(args[1]) > items){
                exitMsg.Error("HandleCall.At", "segfault", arguments, 1);
            }

            std::string arrayIndexer = preArray + "[" + args[1] + "]";
            std::string arrayItem = Token::getToken(arrayIndexer).value;

            Token returnToken;
            returnToken.dataType = Token::t_string;
            returnToken.value = arrayItem;
            return returnToken;
        }
        case Length:{
            if (!Token::tokenExists(arguments)){
                exitMsg.Error("HandleCall.Length", arguments + " is not a defined array variable!", arguments, 1);
            }
            if (!Token::IsArray(Token::getAllTokens(arguments))){
                exitMsg.Error("HandleCall.Length", arguments + " is not of type array!", arguments, 1);
            }

            Token::ConvertToTokenValue(arguments);
            int items = std::count(arguments.begin(),  arguments.end(), ',') + 1;

            Token returnToken;
            returnToken.dataType = Token::t_integer;
            returnToken.value = std::to_string(items);
            return returnToken;
        }
    }
}
