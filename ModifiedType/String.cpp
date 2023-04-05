//
// Created by mary on 4/5/2023.
//


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

String String::Replace(const std::string& oldValue, std::string newValue) {
    std::unique_ptr<String> ptrString = std::make_unique<String>(content);
    size_t pos = ptrString->content.find(oldValue);
    size_t size = oldValue.size();
    return String(ptrString->content.replace(pos, size, newValue));
}

String String::Replace(const String& oldValue, String newValue) {
    std::unique_ptr<String> ptrContent = std::make_unique<String>(content);
    size_t pos = ptrContent->content.find(oldValue.content);
    size_t size = oldValue.content.size();
    return String(ptrContent->content.replace(pos, size, newValue.content));
}

std::string String::ToString() {
    return content;
}
