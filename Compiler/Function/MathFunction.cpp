//
// Created by mary on 3/6/2023.
//

#include <unordered_map>
#include "MathFunction.h"


MathFunction::MethodFunctions GetMathFunction(std::string const& str){
    const std::unordered_map<std::string, MathFunction::MethodFunctions> functionTable{
            { "prime", MathFunction::MethodFunctions::IsPrime},
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

Variable MathFunction::HandleCall(std::string function, std::string arguments) {
    Variable emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.type = Variable::t_empty;

    switch (GetMathFunction(function)){
        case Modulo:{

            std::vector<std::string> args = StringExt::Split(arguments, ",");
            Variable::CleanTokens(args);
            Variable::ConvertToTokenValues(args);

            int left = StringExt::ToInteger(args[0]);
            int right = StringExt::ToInteger(args[1]);

            int result = left % right;

            Variable moduloToken;
            moduloToken.value = std::to_string(result);
            moduloToken.type = Variable::t_integer;

            return moduloToken;
        }
        case Null:
            break;
        case IsPrime:
            Variable::ConvertToTokenValue(arguments);

            Variable token;
            token.type = Variable::t_boolean;

            int n = StringExt::ToInteger(arguments);
            if (n == 0 || n == 1){
                token.value = "false";
                return token;
            }

            for (int i = 0; i < n; i++){
                if (n % i == 0){
                    token.value = "false";
                    return token;
                }
            }

            token.value = "true";
            return token;
    }
}
