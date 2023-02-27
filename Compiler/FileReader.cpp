//
// Created by bobwt on 2/10/2023.
//

#include <fstream>
#include "FileReader.h"
#include "../Extensions/String.h"

// Does not include comments of any sort
std::vector<std::string> FileReader::Read(std::string fileName) {
    std::string line;
    std::ifstream fstream(fileName);

    std::vector<std::string> vector;

    while (std::getline(fstream, line)){
        vector.push_back(line);
    }

    return vector;
}
