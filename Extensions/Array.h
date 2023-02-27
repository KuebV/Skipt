//
// Created by bobwt on 2/9/2023.
//

#ifndef SKIPT_ARRAY_H
#define SKIPT_ARRAY_H

#include <string>
#include <vector>
#include <array>

class Array{
public:
    static bool Contains(std::vector<int> arr, int element);
    static bool Contains(std::vector<double> arr, double element);
    static bool Contains(std::vector<std::string> arr, const std::string &element);
};

bool Array::Contains(const std::vector<int> arr, int element) {
    for (int i : arr){
        if (i == element)
            return true;
    }
    return false;
}

bool Array::Contains(const std::vector<double> arr, double element) {
    for (double i : arr){
        if (i == element)
            return true;
    }
    return false;
}

bool Array::Contains(const std::vector<std::string> arr, const std::string& element) {
    for (const auto & i : arr){
        if (i == element)
            return true;
    }
    return false;
}

#endif //SKIPT_ARRAY_H
