//
// Created by mary on 4/5/2023.
//

#ifndef SKIPT_STRING_H
#define SKIPT_STRING_H


#include <string>
#include <vector>
#include <memory>

class String {
public:
    explicit String(const std::string& string){
        content = string;
        Length = string.length();
    }

    int Length;
    bool Contains(std::string search);
    bool Contains(String search);
    bool Contains(char search);

    String Substring(int startIndex, int length);

    String Replace(const std::string &oldValue, std::string newValue);
    String Replace(const String &oldValue, String newValue);

    std::string ToString();


private:
    std::string content;
};


#endif //SKIPT_STRING_H
