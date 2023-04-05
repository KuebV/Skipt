//
// Created by bobwt on 3/4/2023.
//

#include <unordered_map>
#include <utility>
#include "InternalFunction.h"
#include "../Compile.h"
#include "../../Property/PropertyFile.h"
#include "../ExitMessage.h"


InternalFunction::MethodFunctions GetInternalFunction(std::string const& str){
    const std::unordered_map<std::string, InternalFunction::MethodFunctions> functionTable{
            { "execute_skipt", InternalFunction::MethodFunctions::RunSkiptFile},
            { "print_tokens", InternalFunction::MethodFunctions::ListTokens},
            { "convert", InternalFunction::MethodFunctions::Convert}
    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return InternalFunction::MethodFunctions::Null;
    }
}


Variable::dataTypes StringToType(std::string specifiedType){
    const std::unordered_map<std::string, Variable::dataTypes> typeTable{
            { "int",    Variable::dataTypes::t_integer},
            { "double", Variable::dataTypes::t_integer},
            { "bool",   Variable::dataTypes::t_integer},
            { "string", Variable::dataTypes::t_integer},
            { "float",  Variable::dataTypes::t_float}
    };
    auto it = typeTable.find(specifiedType);
    if (it != typeTable.end())
        return it->second;
    else{
        std::cout << "[Error] | [InternalFucnction.cpp] [StringToType] : Invalid Type Conversion!\n";
        return Variable::dataTypes::t_empty;
    }
}


Variable InternalFunction::HandleCall(std::string function, std::string arguments) {
    Variable emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Variable::t_empty;

    ExitMessage exitMsg = ExitMessage("InternalFunction.cpp");

    switch (GetInternalFunction(function)){
        case RunSkiptFile:{
            Variable::ConvertToTokenValue(arguments);
            if (!PropertyFile::fileExists(arguments)){
                std::cout << "[Error] | [InternalFunction.cpp] [Execute-Skipt]: File does not exist!\n";
                std::cout << "        |> " << arguments << "\n";
                exit(1);
            }

            Compile functionCompiler;
            functionCompiler.Run(std::move(arguments), true);
            return emptyToken;
        }
        case ListTokens:
            for (auto it = Variable::variableMap.begin(); it != Variable::variableMap.end(); it++){
                Variable tkn = it->second;
                std::cout << "Name : " << tkn.name << "\tValue: " << tkn.value << "\tAddress: " << &it->second << "\tSizeof: " << sizeof(it->second) <<"\n";
            }
            return emptyToken;;

        case Null:
            break;
    }
}

