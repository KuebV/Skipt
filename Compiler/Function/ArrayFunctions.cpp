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


Variable ArrayFunctions::HandleCall(std::string function, std::string arguments) {
    Variable emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Variable::t_empty;

    ExitMessage exitMsg = ExitMessage("ArrayFunctions.cpp");

    switch (GetArrayFunction(function)){
        case At:{
            // Arguments
            // Array, Index
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);

            if (!Variable::variableExists(args[0])){
                exitMsg.Error("HandleCall.At", args[0] + " is not a defined array variable!", arguments, 1);
            }
            std::string preArray = args[0];

            Variable::ConvertToTokenValues(args);

            if (!StringExt::IsInteger(args[1])){
                exitMsg.Error("HandleCall.At", args[1] + " is expected to be of type integer!", arguments, 1);
            }
            int items = std::count(args[0].begin(),  args[0].end(), ',');

            if (StringExt::ToInteger(args[1]) > items){
                exitMsg.Error("HandleCall.At", "segfault", arguments, 1);
            }

            std::string arrayIndexer = preArray + "[" + args[1] + "]";
            std::string arrayItem = Variable::getVariable(arrayIndexer).value;

            Variable returnToken;
            returnToken.dataType = Variable::t_string;
            returnToken.value = arrayItem;
            return returnToken;
        }
        case Length:{
            if (!Variable::variableExists(arguments)){
                exitMsg.Error("HandleCall.Length", arguments + " is not a defined array variable!", arguments, 1);
            }
            if (!Variable::IsArray(Variable::getAllVariables(arguments))){
                exitMsg.Error("HandleCall.Length", arguments + " is not of type array!", arguments, 1);
            }

            Variable::ConvertToTokenValue(arguments);
            int items = std::count(arguments.begin(),  arguments.end(), ',') + 1;

            Variable returnToken;
            returnToken.dataType = Variable::t_integer;
            returnToken.value = std::to_string(items);
            return returnToken;
        }
    }
}
