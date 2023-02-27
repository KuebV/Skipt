//
// Created by bobwt on 2/9/2023.
//

#ifndef SKIPT_PROPERTYFILE_H
#define SKIPT_PROPERTYFILE_H

#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#include "Property.h"
#include "PropertyReference.h"

class PropertyFile {
public:
    static void WritePropertyFile(std::string fileName, std::vector<Property> properties);
    static bool fileExists(const std::string& fileName);
    static PropertyReference ReadPropertyFile(std::string _file);
};


#endif //SKIPT_PROPERTYFILE_H
