//
// Created by bobwt on 3/4/2023.
//

#include <unordered_map>
#include <utility>
#include "InternalFunction.h"
#include "../Compile.h"
#include "../../Property/PropertyFile.h"

InternalFunction::MethodFunctions GetInternalFunction(std::string const& str){
    const std::unordered_map<std::string, InternalFunction::MethodFunctions> functionTable{
            { "execute_skipt", InternalFunction::MethodFunctions::RunSkiptFile},
            { "print_tokens", InternalFunction::MethodFunctions::ListTokens},
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


Token InternalFunction::HandleCall(std::string function, std::string arguments) {
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    switch (GetInternalFunction(function)){
        case RunSkiptFile:{
            if (!PropertyFile::fileExists(arguments)){
                std::cout << "[Error] | [InternalFunction.cpp] [Execute-Skipt]: File does not exist!\n";
                std::cout << "        |> " << arguments << "\n";
                exit(1);
            }

            Compile functionCompiler;
            functionCompiler.Run(std::move(arguments), false);
            return emptyToken;
        }
        case ListTokens:
            for (auto it = Token::tokenMap.begin(); it != Token::tokenMap.end(); it++){
                Token tkn = it->second;
                std::cout << "Name : " << tkn.name << "\tValue: " << tkn.value << "\tAddress: " << &it->second << "\n";
            }
            return emptyToken;
        case Null:
            break;
    }
}

