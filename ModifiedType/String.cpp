//
// Created by mary on 4/5/2023.
//


#include <iostream>
#include "String.h"

bool String::Contains(std::string search) {
    if (content.find(search) != std::string::npos){
        return true;
    }
    return false;
}

bool String::Contains(char search) {
    if (content.find(search) != std::string::npos){
        return true;
    }
    return false;
}


bool String::Contains(String search) {
    if (content.find(search.content) != std::string::npos){
        return true;
    }
    return false;
}

String String::Substring(int startIndex, int length) {
    return String(content.substr(startIndex, length));
}

String String::ReplaceSingle(std::string oldValue, std::string newValue) {
    std::unique_ptr<String> ptrString = std::make_unique<String>(content);
    size_t pos = ptrString->content.find(oldValue);
    size_t size = oldValue.size();
    return String(ptrString->content.replace(pos, size, newValue));
}

String String::ReplaceSingle(String oldValue, String newValue) {
    std::unique_ptr<String> ptrContent = std::make_unique<String>(content);
    size_t pos = ptrContent->content.find(oldValue.content);
    size_t size = oldValue.content.size();
    return String(ptrContent->content.replace(pos, size, newValue.content));
}

void String::ptr_ReplaceSingle(std::string oldValue, std::string newValue) {
    size_t pos = content.find(oldValue);
    size_t size = oldValue.size();
    content.replace(pos, size, newValue);
}

void String::ptr_ReplaceSingle(String oldValue, String newValue) {
    size_t pos = content.find(oldValue.content);
    size_t size = oldValue.content.size();
    content.replace(pos, size, newValue.content);
}

String String::ReplaceMulti(std::string oldValue, std::string newValue) {
    std::unique_ptr<size_t> startPos = std::make_unique<size_t>();
    std::unique_ptr<std::string> ptrContent = std::make_unique<std::string>(content);
    while ((*startPos = content.find(oldValue, *startPos)) != std::string::npos){
        ptrContent->replace(*startPos, oldValue.length(), newValue);
        *startPos += newValue.length();
    }
    return String(*ptrContent);
}

String String::ReplaceMulti(String oldValue, String newValue) {
    std::unique_ptr<size_t> startPos = std::make_unique<size_t>();
    std::unique_ptr<std::string> ptrContent = std::make_unique<std::string>(content);
    while ((*startPos = ptrContent->find(oldValue.content, *startPos)) != std::string::npos){
        ptrContent->replace(*startPos, oldValue.content.length(), newValue.content);
        *startPos += newValue.content.length();
    }
    return String(*ptrContent);
}

void String::ptr_ReplaceMulti(std::string oldValue, std::string newValue) {
    std::unique_ptr<size_t> startPos = std::make_unique<size_t>();
    std::unique_ptr<std::string> ptrContent = std::make_unique<std::string>(content);
    while ((*startPos = ptrContent->find(oldValue, *startPos)) != std::string::npos){
        ptrContent->replace(*startPos, oldValue.length(), newValue);
        *startPos += newValue.length();
    }
    content = *ptrContent;
}

void String::ptr_ReplaceMulti(String oldValue, String newValue) {
    std::unique_ptr<size_t> startPos = std::make_unique<size_t>();
    std::unique_ptr<std::string> ptrContent = std::make_unique<std::string>(content);
    while ((*startPos = ptrContent->find(oldValue.content, *startPos)) != std::string::npos){
        ptrContent->replace(*startPos, oldValue.content.length(), newValue.content);
        *startPos += newValue.content.length();
    }
    content = *ptrContent;
}

std::vector<String> String::Split(const std::string delimiter){
    size_t pos_start = 0, pos_end, delimter_end = delimiter.length();
    std::unique_ptr<std::string> item = std::make_unique<std::string>();
    std::unique_ptr<std::vector<String>> vector = std::make_unique<std::vector<String>>();

    while ((pos_end = content.find (delimiter, pos_start)) != std::string::npos) {
        *item = content.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delimter_end;
        vector->push_back(String(*item));
    }
    vector->push_back(String(content.substr(pos_start)));
    return *vector;
}


std::string String::ToString() {
    return content;
}

String String::ContentBetween(std::string startingElement, std::string endingElement) {
    std::unique_ptr<size_t> startElement = std::make_unique<size_t>(content.find(startingElement));
    if (*startElement == std::string::npos){
        return String("");
    }

    std::unique_ptr<size_t> endElement = std::make_unique<size_t>(content.find_last_of(endingElement));
    if (*endElement == std::string::npos){
        return String("");
    }
    return String(content.substr(*startElement + 1, (*endElement - *startElement) - 1));
}

String String::ReplaceLiterals() {
    std::unique_ptr<std::string> ptrString = std::make_unique<std::string>(content);
    for (int i = 0; i < content.length(); i++){
        char currentChar = content[i];
        if (currentChar == '\\'){
            switch (content[i + 1]){
                case 'n':{
                    ptr_ReplaceMulti("\\n", "\n");
                    break;
                }
            }
        }
    }

    return String(content);
}

int String::Count(std::string element) {
    int n = 0;
    std::string::size_type pos = 0;
    while ((pos = content.find(element, pos)) != std::string::npos){
        n++;
        pos += element.length();
    }
    return n;
}

size_t String::Until(char untilChar) {
    return content.find(untilChar);
}

size_t String::Until(std::string untilString) {
    return content.find(untilString);
}

size_t String::Until(String untilString) {
    return content.find(untilString.content);
}

bool String::ContainsAny(std::vector<std::string> searchElements) {
    for (int i = 0; i < searchElements.size(); i++){
        if (content.find(searchElements[i]) != std::string::npos)
            return true;
    }
    return false;
}

String String::Strip(StripOptions options){
    switch (options){
        case StripOptions::Beginning:{
            std::unique_ptr<std::string> tempString = std::make_unique<std::string>("");
            int alphaNumIndex = 0;
            for (int i = 0; i < content.length(); i++){
                if (std::isspace(content[i])){
                    alphaNumIndex = i;
                    break;
                }
            }

            *tempString = content.substr(alphaNumIndex, (content.length() - alphaNumIndex));
            return String(*tempString);
        }
        case StripOptions::All:{
            std::unique_ptr<std::string> tempString = std::make_unique<std::string>("");
            for (int i = 0; i < content.length(); i++){
                if (!std::isspace(content[i]))
                    *tempString += content[i];
            }
            return String(*tempString);
        }
    }
}

void String::ptr_Strip(String::StripOptions options) {
    switch (options){
        case StripOptions::Beginning:{
            std::unique_ptr<std::string> tempString = std::make_unique<std::string>("");
            int alphaNumIndex = 0;
            for (int i = 0; i < content.length(); i++){
                if (std::isspace(content[i])){
                    alphaNumIndex = i;
                    break;
                }
            }

            *Ptr_content = Ptr_content->substr(alphaNumIndex, (content.length() - alphaNumIndex));
        }
        case StripOptions::All:{
            std::unique_ptr<std::string> tempString = std::make_unique<std::string>("");
            for (int i = 0; i < content.length(); i++){
                if (!std::isspace(content[i]))
                    *tempString += content[i];
            }

            Ptr_content = std::make_unique<std::string>(*tempString);
            content = *tempString;
        }
    }
}

void String::MakeUnique(std::string string) {
    Ptr_content = std::make_unique<std::string>(string);
}




