//
// Created by mary on 3/6/2023.
//

#include <unordered_map>
#include "MathFunction.h"

MathFunction::MethodFunctions GetMathFunction(std::string const& str){
    const std::unordered_map<std::string, MathFunction::MethodFunctions> functionTable{
            { "isprime", MathFunction::MethodFunctions::IsPrime},
            { "modulo", MathFunction::MethodFunctions::Modulo},
            { "mod", MathFunction::MethodFunctions::Modulo},
    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return MathFunction::MethodFunctions::Null;
    }
}

Token MathFunction::HandleCall(std::string function, std::string arguments) {
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    switch (GetMathFunction(function)){
        case Modulo:{

            std::vector<std::string> args = String::Split(arguments, ",");
            for (int i = 0; i < args.size(); i++){
                args[i] = String::Strip(args[i]);
            }

            int left = String::ToInteger(args[0]);
            int right = String::ToInteger(args[1]);

            int result = left % right;

            Token moduloToken;
            moduloToken.value = std::to_string(result);
            moduloToken.dataType = Token::t_integer;

            return moduloToken;
        }
    }
}
