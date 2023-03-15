//
// Created by bobwt on 2/28/2023.
//

#include <unordered_map>
#include <utility>
#include <fstream>
#include "SystemFunction.h"


SystemFunction::MethodFunctions GetSystemFunction(std::string const& str){
    const std::unordered_map<std::string, SystemFunction::MethodFunctions> functionTable{
            { "exit", SystemFunction::MethodFunctions::Exit},
            { "file_create", SystemFunction::MethodFunctions::FileCreate},
            { "file_delete", SystemFunction::MethodFunctions::FileDelete},
            { "file_write", SystemFunction::MethodFunctions::FileWrite},
            { "file_read", SystemFunction::MethodFunctions::FileRead},
            { "file_append", SystemFunction::MethodFunctions::FileAppend},
            { "file_exists", SystemFunction::MethodFunctions::FileExists},
            { "out", SystemFunction::MethodFunctions::Out}
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
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    switch (GetSystemFunction(function)){
        case Exit:{
            Token::ConvertToTokenValue(arguments);
            exit(String::ToInteger(std::move(arguments)));
        }
        case FileCreate:{
            Token::ConvertToTokenValue(arguments);
            std::ofstream fstream;
            fstream.open(arguments);
            fstream.close();
            return emptyToken;
        }
        case FileExists:{
            Token::ConvertToTokenValue(arguments);
            Token returnFileExistsToken;
            returnFileExistsToken.name = "booleanFileExists";
            returnFileExistsToken.dataType = Token::t_boolean;
            if (FILE *p_file = fopen(arguments.c_str(), "r")){
                fclose(p_file);
                returnFileExistsToken.value = "true";
            }
            else{
                returnFileExistsToken.value = "false";
            }
            return returnFileExistsToken;
        }
        case FileDelete:{
            Token::ConvertToTokenValue(arguments);
            std::remove(arguments.c_str());
            return emptyToken;
        }
        case FileWrite:
            break;
        case FileRead:
            break;
        case FileAppend:
            break;
        case Out:{

        }
        case Null:
            break;

    }
    return {};
}
