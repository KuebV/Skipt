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
        Property ExecutionTime("executionTime", "false", "Gets the time that the application takes to fully execute");
        Property KeepOpen("keepOpen", "true", "When the compiler is finished, should the executable wait until the user presses ENTER?");

        std::vector<Property> properties = {SkiptFile, GenerateNewFile, DebugMode, GarbageCollection, ExecutionTime, KeepOpen};
        PropertyFile::WritePropertyFile("compiler.properties", properties);
    }

    // In the compiler.properties file, if the GenerateNewFile property is true, then the file will be auto generated
    static void WriteDefaultGeneratedFile(std::string fileName){
        std::ofstream fstream;
        fstream.open(fileName);

        if (fstream.is_open()){
            fstream << "# Comments are indicated by the \'#\' character at the start of each line. They cannot be put at the end of a statement.\n";
            fstream << "# You can define variables by using the following data-types\n";
            fstream << "# int, double, string, bool, int[], double[], string[], bool[]\n";
            fstream << "# Why you would ever want to make an array of booleans is beyond me\n";
            fstream << "# You must define a variable first before handling any sort of user input, calculation, or reference\n";
            fstream << "# You're able to compute mathmatical expressions by doing the following\n";
            fstream << "# int x = 0\n";
            fstream << "# x = x + 2\n";
            fstream << "# With Skipt, we're able to use conditional statements such as if-statements, and while-statements\n";
            fstream << "# Please visit the Github to learn about proper-code styling\n";

            fstream.flush();
            fstream.close();
        }
        else{
            std::cout << "Unable to generate default file\n";
        }

    }
};

#endif //SKIPT_DEFAULTFILES_H
