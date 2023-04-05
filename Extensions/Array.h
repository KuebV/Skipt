//
// Created by mary on 4/5/2023.
//

#ifndef SKIPT_ARRAY_H
#define SKIPT_ARRAY_H

#include <string>
#include <vector>

class Array {
    static bool Contains(std::vector<std::string> arr, const std::string &element);

public:
    static bool Contains(std::vector<int> arr, int element);

    static bool Contains(std::vector<double> arr, double element);
};


#endif //SKIPT_ARRAY_H
