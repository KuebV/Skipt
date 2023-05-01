//
// Created by mary on 4/5/2023.
//

#ifndef SKIPT_STRING_H
#define SKIPT_STRING_H


#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class String {
public:

    enum StripOptions{
        Beginning,
        All
    };

    explicit String(const std::string& string){
        content = string;
        Length = string.length();

        Ptr_content = std::make_unique<std::string>(content);
    }

    String(const String &string){
        content = string.content;
        Length = string.content.length();

        Ptr_content = std::make_unique<std::string>(content);
    }

    int Length;
    bool Contains(std::string search);
    bool Contains(String search);
    bool Contains(char search);

    String Substring(int startIndex, int length);

    String ReplaceSingle(std::string oldValue, std::string newValue);
    String ReplaceSingle(String oldValue, String newValue);

    void ptr_ReplaceSingle(std::string oldValue, std::string newValue);
    void ptr_ReplaceSingle(String oldValue, String newValue);

    String ReplaceMulti(std::string oldValue, std::string newValue);
    String ReplaceMulti(String oldValue, String newValue);

    void ptr_ReplaceMulti(std::string oldValue, std::string newValue);
    void ptr_ReplaceMulti(String oldValue, String newValue);

    String ContentBetween(std::string startingElement, std::string endingElement);
    void ptr_ContentBetween(std::string startingElement, std::string endingElement);
    String ReplaceLiterals();

    size_t Until(char untilChar);
    size_t Until(std::string untilString);
    size_t Until(String untilString);

    std::string ToString();

    std::vector<String> Split(const std::string delimiter);
    int Count(std::string element);

    bool ContainsAny(std::vector<std::string> searchElements);
    String Strip(StripOptions stripOptions);
    void ptr_Strip(StripOptions stripOptions);

    void MakeUnique(std::string string);

    bool StartsWith(const char& c);

    bool EndsWith(const char& c);



private:
    std::unique_ptr<std::string> Ptr_content;
    std::string content;
};


#endif //SKIPT_STRING_H
