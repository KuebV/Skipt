//
// Created by bobwt on 2/28/2023.
//

#include <unordered_map>
#include <utility>
#include <fstream>
#include "SystemFunction.h"
#include "../ExitMessage.h"


SystemFunction::MethodFunctions GetSystemFunction(std::string const& str){
    const std::unordered_map<std::string, SystemFunction::MethodFunctions> functionTable{
            { "exit", SystemFunction::MethodFunctions::Exit},
            { "file_create", SystemFunction::MethodFunctions::FileCreate},
            { "file_delete", SystemFunction::MethodFunctions::FileDelete},
            { "file_write", SystemFunction::MethodFunctions::FileWrite},
            { "file_read", SystemFunction::MethodFunctions::FileRead},
            { "file_append", SystemFunction::MethodFunctions::FileAppend},
            { "file_exists", SystemFunction::MethodFunctions::FileExists},
            { "out", SystemFunction::MethodFunctions::Out},
            { "randint", SystemFunction::MethodFunctions::RandInt},
            { "rand", SystemFunction::MethodFunctions::InitRand},
            { "randelement", SystemFunction::MethodFunctions::RandArray}
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

    ExitMessage exitMessage = ExitMessage("SystemFunction.cpp");

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
        case InitRand:{
            Token::ConvertToTokenValue(arguments);
            if (!String::IsInteger(arguments)){
                exitMessage.Error("rand", "Random Seed is not of type integer!", arguments, 1);
            }
            Token::DefineVariable("System::Random(RandomSeed)", arguments, Token::t_integer, false);
            break;
        }
        case RandInt:{
            std::vector<std::string> args = String::Split(arguments, ",");

            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

            if (!Token::tokenExists("System::Random(RandomSeed)")){
                exitMessage.Error("randint", "Random Seed was not initialized!", arguments, 1);
            }

            if (String::ToInteger(Token::getToken("System::Random(RandomSeed)").value) == 0) srand(time(NULL));
            else srand(String::ToInteger(Token::getToken("System::Random(RandomSeed)").value));

            int randNum;
            if (!String::IsInteger(args[0]) || !String::IsInteger(args[1])){
                exitMessage.Error("RandInt", "One or more of the arguments is not of type Integer!\n", arguments, 1);
            }


            randNum = rand() % String::ToInteger(args[1]) + String::ToInteger(args[0]);

            Token tkn;
            tkn.value = std::to_string(randNum);
            tkn.dataType = Token::t_integer;

            return tkn;
        }

    }
    return {};
}
