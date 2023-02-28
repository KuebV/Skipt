//
// Created by bobwt on 2/10/2023.
//

#ifndef SKIPT_DEFAULTFILES_H
#define SKIPT_DEFAULTFILES_H

#include <string>
#include <fstream>
#include <vector>

#include "../Property/Property.h"
#include "../Property/PropertyFile.h"

class DefaultFiles{
public:
    static void WriteConfigurationFile(){
        Property SkiptFile("FileToCompile", "example.skipt", "Defines what file that the compiler should read from");
        Property GenerateNewFile("GenerateNewFile", "true", "If the \"FileToCompile\" file is not found, then should the program create a new file with basic arithmetic?");
        Property DebugMode("debugMode", "true", "If enabled, the compiler will return console messages of what exactly it's working on");
        Property GarbageCollection("garbageCollection", "true", "If enabled, the compiler will look ahead of the program and check if the variable is called again or not, if not, the compiler will recycle the variable");
        Property ExitMessage("exitMessage", "Skipt has exited (%code%) with message %message%", "What the compiler prints out to the console when the application is exited");

        std::vector<Property> properties = {SkiptFile, GenerateNewFile, DebugMode, GarbageCollection, ExitMessage};
        PropertyFile::WritePropertyFile("compiler.properties", properties);
    }

    // In the compiler.properties file, if the GenerateNewFile property is true, then the file will be auto generated
    static void WriteDefaultGeneratedFile(std::string fileName){
        std::ofstream fstream;
        fstream.open(fileName);

        if (fstream.is_open()){
            fstream << "# Comments are indicated by the \'#\' character at the start of each line. They cannot be put at the end of a statement.\n";
            fstream << "\n"
                      "# You can define variables, by following the syntax of, data_type:variable_name\n"
                      "# data_type can be a list of things, such as:\n"
                      "# i : integer\n"
                      "# f : float\n"
                      "# d : double\n"
                      "# str : string\n"
                      "# b : bool\n"
                      "# Combining this all together, you can create the following statements:\n\n";
            fstream << "i32:personAge = 19\n";
            fstream << "d:personBalance = 123.45\n\n";

            fstream << "# Strings are defined as a singular double quotation mark at the start of the string, single quotation marks do not work.\n";
            fstream << "str:personAge = \"Rob Thompson\n";

            fstream.flush();
            fstream.close();
        }
        else{
            std::cout << "Unable to generate default file\n";
        }

    }
};

#endif //SKIPT_DEFAULTFILES_H
