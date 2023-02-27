//
// Created by bobwt on 2/9/2023.
//

#ifndef SKIPT_PROPERTYREFERENCE_H
#define SKIPT_PROPERTYREFERENCE_H


#include <string>
#include <map>

class PropertyReference {
public:
    std::string getValue(std::string _key);
    void appendKeyPair(std::string _key, std::string _value);
private:
    std::map<std::string, std::string> KeyPairValue;
};


#endif //SKIPT_PROPERTYREFERENCE_H
