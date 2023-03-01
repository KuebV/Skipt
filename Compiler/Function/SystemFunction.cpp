//
// Created by bobwt on 2/28/2023.
//

#include <unordered_map>
#include <utility>
#include "SystemFunction.h"

SystemFunction::MethodFunctions GetSystemFunction(std::string const& str){
    const std::unordered_map<std::string, SystemFunction::MethodFunctions> functionTable{
            { "exit", SystemFunction::MethodFunctions::Exit},
            { "file_create", SystemFunction::MethodFunctions::FileCreate},
            { "file_delete", SystemFunction::MethodFunctions::FileDelete},
            { "file_write", SystemFunction::MethodFunctions::FileWrite},
            { "file_read", SystemFunction::MethodFunctions::FileRead},
            { "file_append", SystemFunction::MethodFunctions::FileAppend}
    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return SystemFunction::MethodFunctions::Null;
    }
}

Token SystemFunction::HandleCall(std::string function, std::string arguments) {
    switch (GetSystemFunction(function)){
        case Exit:{
            exit(String::ToInteger(std::move(arguments)));
            return {};
        }
    }
    return {};
}
