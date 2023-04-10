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

Variable SystemFunction::HandleCall(std::string function, std::string arguments) {
    Variable emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Variable::t_empty;

    ExitMessage exitMessage = ExitMessage("SystemFunction.cpp");

    switch (GetSystemFunction(function)){
        case Exit:{
            Variable::ConvertToTokenValue(arguments);
            exit(StringExt::ToInteger(std::move(arguments)));
        }
        case FileCreate:{
            Variable::ConvertToTokenValue(arguments);
            std::ofstream fstream;
            fstream.open(arguments);
            fstream.close();
            return emptyToken;
        }
        case FileExists:{
            Variable::ConvertToTokenValue(arguments);
            Variable returnFileExistsToken;
            returnFileExistsToken.name = "booleanFileExists";
            returnFileExistsToken.dataType = Variable::t_boolean;
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
            Variable::ConvertToTokenValue(arguments);
            std::remove(arguments.c_str());
            return emptyToken;
        }
        case FileWrite:
            break;
        case FileRead:{
            Variable fileContents;
            fileContents.dataType = Variable::t_strArray;
            break;
        }

        case FileAppend:
            break;
        case Out:{

            std::string stringSubstring = StringExt::Substring(arguments, "\"", "\"");
            arguments = StringExt::Replace(arguments, "\"" + stringSubstring + "\"", "");
            arguments = StringExt::NonAlphaStrip(arguments);

            std::vector<std::string> args = StringExt::Split(arguments, ",");

            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            std::string initialString = stringSubstring;
            for (int i = 0; i < args.size();i++){
                std::string formattingBracket = "{" + std::to_string(i) + "}";
                std::string awahhh = args[i];
                initialString = StringExt::Replace(initialString, "{" + std::to_string(i) + "}", args[i]);
            }

            initialString = StringExt::ReplaceLiterals(initialString);
            std::string x = initialString;

            std::cout << initialString;
            return emptyToken;
        }
        case Null:
            break;
        case InitRand:{
            Variable::ConvertToTokenValue(arguments);
            if (!StringExt::IsInteger(arguments)){
                exitMessage.Error("rand", "Random Seed is not of type integer!", arguments, 1);
            }
            Variable::DefineVariable("System::Random(RandomSeed)", arguments, Variable::t_integer, false);
            break;
        }
        case RandInt:{
            std::vector<std::string> args = StringExt::Split(arguments, ",");

            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            if (!Variable::Exists("System::Random(RandomSeed)")){
                exitMessage.Error("randint", "Random Seed was not initialized!", arguments, 1);
            }

            if (StringExt::ToInteger(Variable::Get("System::Random(RandomSeed)").value) == 0) srand(time(NULL));
            else srand(StringExt::ToInteger(Variable::Get("System::Random(RandomSeed)").value));

            int randNum;
            if (!StringExt::IsInteger(args[0]) || !StringExt::IsInteger(args[1])){
                exitMessage.Error("RandInt", "One or more of the arguments is not of type Integer!\n", arguments, 1);
            }


            randNum = rand() % StringExt::ToInteger(args[1]) + StringExt::ToInteger(args[0]);

            Variable tkn;
            tkn.value = std::to_string(randNum);
            tkn.dataType = Variable::t_integer;

            return tkn;
        }

    }
    return {};
}
