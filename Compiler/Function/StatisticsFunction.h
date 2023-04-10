//
// Created by Rob on 4/6/2023.
//

#ifndef SKIPT_STATISTICSFUNCTION_H
#define SKIPT_STATISTICSFUNCTION_H


#include "../Variable.h"

class StatisticsFunction {
public:
    enum MethodFunctions{
        StandardDeviation,
        Mean,
        Median,
        Frequency,
        QuartileOne,
        QuartileThree,
        Null
    };

    static Variable HandleCall(std::string function, std::string arguments);
};


#endif //SKIPT_STATISTICSFUNCTION_H
