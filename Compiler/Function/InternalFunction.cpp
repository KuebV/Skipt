//
// Created by bobwt on 3/4/2023.
//

#include <unordered_map>
#include <utility>
#include "InternalFunction.h"
#include "../Compile.h"
#include "../../Property/PropertyFile.h"
#include "../ExitMessage.h"


InternalFunction::MethodFunctions GetStatisticalFunction(std::string const& str){
    const std::unordered_map<std::string, InternalFunction::MethodFunctions> functionTable{
            { "execute_skipt", InternalFunction::MethodFunctions::RunSkiptFile},
            { "list_variables", InternalFunction::MethodFunctions::ListVariables},
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
    emptyToken.type = Variable::t_empty;

    ExitMessage exitMsg = ExitMessage("InternalFunction.cpp");

    switch (GetStatisticalFunction(function)){
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
        case ListVariables:
            for (auto it = Variable::variableMap.begin(); it != Variable::variableMap.end(); it++){
                Variable tkn = it->second;
                std::cout << "Name : " << tkn.name << "\tValue: " << tkn.value << "\tAddress: " << &it->second << "\tData_Type: " << Variable::EnumToString(it->second.type) << "\n";
            }
            return emptyToken;;
        case Convert:{
            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);

            // ref Internal::convert(variable, type)
            if (!Variable::Exists(args[0])){
                exitMsg.Error("ConvertVariable", "Variable does not exist!", arguments, 1);
            }
            std::unique_ptr<Variable> variable = std::make_unique<Variable>(Variable::Get(args[0]));
            Variable::dataTypes oldDatatype = variable->type;
            Variable::dataTypes newDatatype = StringToType(args[1]);
            if (newDatatype == Variable::t_empty){
                exitMsg.Error("ConvertVariable", "Datatype does not exist!", arguments, 1);
            }

            if (Variable::IsArray(newDatatype) || Variable::IsArray(oldDatatype)){
                exitMsg.Error("ConvertVariable", "You cannot recast variables if they are arrays", arguments, 1);
            }

            switch (newDatatype){
                case Variable::t_integer:{
                    int n = StringExt::ToInteger(variable->value);
                    Variable::variableMap[variable->name].type = newDatatype;
                    Variable::variableMap[variable->name].value = std::to_string(n);
                    break;
                }
                case Variable::t_double:{
                    Variable::variableMap[variable->name].type = newDatatype;
                    break;
                }
                case Variable::t_string:{
                    Variable::variableMap[variable->name].type = newDatatype;
                    break;
                }
            }
        }
        case Null:
            break;
    }
}

