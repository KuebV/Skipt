//
// Created by bobwt on 2/10/2023.
//

#include <unordered_map>
#include "ExpressionParser.h"
#include "../Extensions/StringExt.h"


std::string ExpressionParser::ReplaceVariableNames(std::string line) {
    std::string temp = line;

    std::unordered_map<std::string, double> map;
    map = {{"MATH.PI", 3.1415926535},
           {"MATH.PI_LONG", 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844},
           {"MATH.EULER", 2.71828},
           {"MATH.GOLDEN_RATIO", 1.61803398874989484820458683436563811}};

    std::vector<std::string> values = StringExt::Split(line, " ");
    for (int i = 0; i < values.size(); i++){
        if (Variable::variableExists(values[i])){
            temp = StringExt::Replace(temp, values[i] , Variable::getVariable(values[i]).value);
        }

        if (map.find(values[i]) != map.end()){
            temp = StringExt::Replace(temp, values[i], std::to_string(map[values[i]]));
        }
    }

    return temp;



}
