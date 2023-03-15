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

Token MathFunction::HandleCall(std::string function, std::string arguments) {
    Token emptyToken;
    emptyToken.name = "voidToken";
    emptyToken.value = "";
    emptyToken.dataType = Token::t_empty;

    switch (GetMathFunction(function)){
        case Modulo:{

            std::vector<std::string> args = String::Split(arguments, ",");
            Token::CleanTokens(args);
            Token::ConvertToTokenValues(args);

            int left = String::ToInteger(args[0]);
            int right = String::ToInteger(args[1]);

            int result = left % right;

            Token moduloToken;
            moduloToken.value = std::to_string(result);
            moduloToken.dataType = Token::t_integer;

            return moduloToken;
        }
        case Null:
            break;
        case IsPrime:
            Token::ConvertToTokenValue(arguments);

            Token token;
            token.dataType = Token::t_boolean;

            int n = String::ToInteger(arguments);
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
