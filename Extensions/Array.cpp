//
// Created by mary on 4/5/2023.
//

#include "Array.h"

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
