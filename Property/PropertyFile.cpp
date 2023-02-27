//
// Created by bobwt on 2/9/2023.
//

#include <iostream>
#include "PropertyFile.h"
#include "Property.h"

PropertyReference PropertyFile::ReadPropertyFile(std::string _file) {
    std::ifstream fstream(_file);
    std::string line;

    PropertyReference propRef;

    while (std::getline(fstream, line)){
        if (line[0] == '#')
            continue;

        std::string key;
        std::string divider;
        std::string value;

        std::istringstream iss(line);
        if (iss >> key >> divider >> value){

            // Use this only for debugging purposes
            //std::cout << key << " " << divider << " " << value << "\n";

            // We have to parse this as a whole string
            if (value[0] == '\"'){
                std::size_t dividerValue = line.find(" = \"") + 4; // Apply an offset of 4 characters as not to include = in the final string
                std::string strValue = line.substr(dividerValue);

                value = strValue;
            }
            propRef.appendKeyPair(key, value);
        }
    }

    return propRef;
}

bool PropertyFile::fileExists(const std::string &fileName) {

    // Define p_file as a pointer when opening to a read-only variable
    if (FILE *p_file = fopen(fileName.c_str(), "r")){

        // If we are able to close the file, then we are able to deduce that the file exists
        fclose(p_file);
        return true;
    }
    else{
        // On the contrary, if we aren't able to open the file, then we return false as the file does not exist
        return false;
    }
}

void PropertyFile::WritePropertyFile(std::string fileName, std::vector<Property> properties) {
    std::ofstream fstream;
    fstream.open(fileName);

    for (Property & property : properties){
        fstream << "# " << property.description << "\n";
        fstream << property.key << " = " << property.value << "\n\n";
    }

    fstream.close();
}