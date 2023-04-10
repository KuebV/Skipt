//
// Created by bobwt on 2/10/2023.
//

#include <unordered_map>
#include "ExpressionParser.h"
#include "../Extensions/StringExt.h"
#include "../ModifiedType/String.h"


std::string ExpressionParser::ReplaceVariableNames(std::string line) {
    String temp(line);

    std::unordered_map<std::string, double> map;
    map = {{"MATH.PI", 3.1415926535},
           {"MATH.PI_LONG", 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844},
           {"MATH.EULER", 2.71828},
           {"MATH.GOLDEN_RATIO", 1.61803398874989484820458683436563811}};

    std::vector<std::string> values = StringExt::Split(line, " ");
    for (int i = 0; i < values.size(); i++){
        if (Variable::Exists(values[i])){
            temp.ptr_ReplaceMulti(values[i], Variable::Get(values[i]).value);
        }

        if (map.find(values[i]) != map.end()){
            temp.ptr_ReplaceMulti(values[i], std::to_string(map[values[i]]));
        }
    }

    return temp.ToString();



}
