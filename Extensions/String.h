//
// Created by bobwt on 2/9/2023.
//

#ifndef SKIPT_STRING_H
#define SKIPT_STRING_H

#include <string>
#include <sstream>
#include <vector>

class String{
public:
    static bool IsInteger(const std::string input){
        try{
            std::stoi(input);
            return true;
        }
        catch (std::exception &err){
            return false;
        }
    }

    static int ToInteger(const std::string input){
        return std::stoi(input);
    }

    static bool ToBoolean(const std::string input){
        bool x;
        std::istringstream is(input);
        is >> x;

        if (is.fail()){
            is.clear();
            is >> std::boolalpha >> x;
        }

        return x;
    }

    static bool IsDouble(const std::string input){
        double x;
        std::istringstream iss(input);
        iss >> x;
        if (iss.fail())
            return false;
        return true;
    }

    static double ToDouble(const std::string input){
        double x;
        std::istringstream(input) >> x;
        return x;
    }

    static std::vector<std::string> Split(const std::string inputString, const std::string delimiter){
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> vector;

        while ((pos_end = inputString.find (delimiter, pos_start)) != std::string::npos) {
            token = inputString.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            vector.push_back (token);
        }

        vector.push_back(inputString.substr(pos_start));
        return vector;
    }

    static bool Contains(std::string s1, std::string s2){
        if (s1.find(s2) != std::string::npos){
            return true;
        }
        return false;
    }

    static std::string Replace(std::string s1, std::string stringToReplace, std::string replacer){
        size_t pos;
        while ((pos = s1.find(stringToReplace)) != std::string::npos){
            size_t size = stringToReplace.size();
            s1.replace(pos, size, replacer);
        }

        return s1;
    }

    static std::string Strip(std::string s1){ // Remove any whitespaces in a string, used during the conditional operator
        std::string tempStr = "";
        int alphaNumIndex = 0;
        for (int i = 0; i < s1.length(); i++){
            if (!std::isspace(s1[i])){
                alphaNumIndex = i;
                break;
            }

        }

        tempStr = s1.substr(alphaNumIndex, (s1.length() - alphaNumIndex));
        return tempStr;
    }

    static std::vector<int> ToIntVector(std::string s1){
        std::vector<std::string> elements = String::Split(s1, ",");

        std::vector<int> vector;
        for (int i = 0; i < elements.size(); i++){
            vector.push_back(String::ToInteger(elements[i]));
        }

        return vector;
    }

    static std::vector<double> ToDoubleVector(std::string s1){
        std::vector<std::string> elements = String::Split(s1, ",");

        std::vector<double> vector;
        for (int i = 0; i < elements.size(); i++){
            vector.push_back(String::ToDouble(elements[i]));
        }

        return vector;
    }

    static std::string Join(std::vector<std::string> vec, std::string divider){

        std::string returnString = "";
        for (int i = 0; i < vec.size(); i++){
            returnString += vec[i] + divider;
        }

        return returnString;
    }

    static std::string Substring(std::string line, std::string startString, std::string endString){
        size_t start = line.find(startString);
        if (start == std::string::npos){
            return "";
        }

        size_t end = line.find(endString);
        if (end == std::string::npos){
            return "";
        }
        return line.substr(start + 1, (end - start) - 1);
    }
};

#endif //SKIPT_STRING_H
