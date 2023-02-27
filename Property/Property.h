//
// Created by bobwt on 2/9/2023.
//

#ifndef SKIPT_PROPERTY_H
#define SKIPT_PROPERTY_H

#include <string>

class Property {
public:
    std::string description;
    std::string key;
    std::string value;

    Property(std::string _key, std::string _value, std::string _description){
        key = std::move(_key);
        value = std::move(_value);
        description = std::move(_description);
    }
};

#endif //SKIPT_PROPERTY_H
