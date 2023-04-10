//
// Created by Rob on 4/6/2023.
//

#include <unordered_map>
#include "StatisticsFunction.h"

StatisticsFunction::MethodFunctions GetInternalFunction(std::string const& str){
    const std::unordered_map<std::string, StatisticsFunction::MethodFunctions> functionTable{
            { "mean", StatisticsFunction::MethodFunctions::Mean},
            { "median", StatisticsFunction::MethodFunctions::Median},
            { "q1", StatisticsFunction::MethodFunctions::QuartileOne},
            { "q3", StatisticsFunction::MethodFunctions::QuartileThree},
            { "deviation", StatisticsFunction::MethodFunctions::StandardDeviation},
            { "frequency", StatisticsFunction::MethodFunctions::Frequency},

    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        std::cout << "There is no method function with the name of " << str << "\n";
        return StatisticsFunction::MethodFunctions::Null;
    }
}

Variable StatisticsFunction::HandleCall(std::string function, std::string arguments) {
    return Variable();
}
