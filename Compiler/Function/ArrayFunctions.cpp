//
// Created by Rob on 3/28/2023.
//

#include <unordered_map>
#include <algorithm>
#include "ArrayFunctions.h"
#include "InternalFunction.h"
#include "../ExitMessage.h"
#include "../../Property/PropertyFile.h"
#include "../../ModifiedType/String.h"

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
    emptyToken.type = Variable::t_empty;

    ExitMessage exitMsg = ExitMessage("ArrayFunctions.cpp");

    switch (GetArrayFunction(function)){
        case At:{
            // Arguments
            // Array, Index
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);

            if (!Variable::Exists(args[0])){
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
            std::string arrayItem = Variable::Get(arrayIndexer).value;

            // We need to find the parent array data-type
            // We should be able to do this via subtracting the parent enum by 5
            Variable::dataTypes parentType =  static_cast<Variable::dataTypes>(Variable::Get(preArray).type - 5);

            Variable returnVariable;
            returnVariable.type = parentType;
            returnVariable.value = arrayItem;
            return returnVariable;
        }
        case Length:{
            if (!Variable::Exists(arguments)){
                exitMsg.Error("HandleCall.Length", arguments + " is not a defined array variable!", arguments, 1);
            }
            if (!Variable::IsArray(Variable::Get(arguments))){
                exitMsg.Error("HandleCall.Length", arguments + " is not of type array!", arguments, 1);
            }

            Variable::ConvertToTokenValue(arguments);
            int items = std::count(arguments.begin(),  arguments.end(), ',') + 1;

            Variable returnToken;
            returnToken.type = Variable::t_integer;
            returnToken.value = std::to_string(items);
            return returnToken;
        }
        case Add:{
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);

            if (!Variable::Exists(args[0])){
                exitMsg.Error("HandleCall.At", args[0] + " is not a defined array variable!", arguments, 1);
            }
            std::string preArray = args[0];
            Variable array = Variable::Get(args[0]);
            Variable::dataTypes arrayType = Variable::GetNonArrayType(array.type);
            std::unique_ptr<Variable> valueAppended = std::make_unique<Variable>();
            if (!Variable::Exists(args[1])){
                if (!Variable::ValidateType(args[1], arrayType)){
                    exitMsg.Error("Array.Add", "Variable Data-Type does not match that of the array type!", arguments, 1);
                }

                valueAppended->type = arrayType;
                valueAppended->value = args[1];
            }
            else{
                if (!Variable::ValidateType(Variable::Get(args[1]).value, arrayType)){
                    exitMsg.Error("Array.Add", "Variable Data-Type does not match that of the array type!", arguments, 1);
                }

                valueAppended = std::make_unique<Variable>(Variable::Get(args[1]));
            }
            std::unique_ptr<Variable> variableSizePtr = std::make_unique<Variable>(Variable::Get("Compiler::ArraySize[" + args[0] + "]"));
            std::cout << "\nArray Functions: Array Size: " << variableSizePtr->value << "\n";
            int elements = StringExt::ToInteger(variableSizePtr->value);

            std::unique_ptr<std::string> newArrayElement = std::make_unique<std::string>(preArray + "[" + std::to_string(elements) + "]");
            valueAppended->name = *newArrayElement;

            array.value += ", " + valueAppended->value;
            Variable::modifyVariable(array);
            Variable::DefineVariable(*valueAppended);

            variableSizePtr->value = std::to_string(elements++);
            Variable::modifyVariable(*variableSizePtr);



            return emptyToken;
        }
    }
}
