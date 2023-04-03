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

        std::ofstream docStream;
        docStream.open("documentation.md");
        if (docStream.is_open()){
            docStream << "# Skipt Practices & Documentation\n";
            docStream << "## Skipt is a interpreted language with the main goal to having a relatively easy to use syntax and having the same performance as other languages. \n"
                         "Additionally, the language is designed to have the user have complete control of their variables and program\n";
            docStream << "### Data-Types\n- int\n- double\n- string\n- bool\n";
            docStream << "### Array Types\n- int[]\n- double[]\n- string[]\n- bool[]\n\n";
            docStream << "**Example Variable & Output**\n"
                         "```\n"
                         "# This indicates a comment\n"
                         "\n"
                         "string name = \"Robert Thompson\"\n"
                         "int age = 19;\n"
                         "# ^ Skipt doesn't care whether or not you end your lines of code with semi-colons or not, they are processed the same\n"
                         "\n"
                         "> name\n"
                         "> \n"
                         "> age\n"
                         "```\n";
            docStream << "\n"
                         "### Input & Output\n"
                         "Skipt contains two types of outputs, one is the traditional output, corresponding to the `>` character, this will output any variable that is given\n"
                         "\n"
                         "A non-traditional output can be done via `ref System::out()` this contains output formatting. An example of this can be shown below:\n"
                         "\n"
                         "```\n"
                         "# Traditional Output\n"
                         "int favoriteNumber = 42;\n"
                         "> \"Your favorite number is \"\n"
                         "> favoriteNumber\n"
                         "\n"
                         "# Non Traditional Output\n"
                         "int favoriteNumber = 42;\n"
                         "ref System::out(\"Your favorite number is {0}\"\\, favoriteNumber)\n"
                         "```";
            docStream << "Skipt only contains 1 type of input, this corresponds to the `<` character, followed shortly by a variable. Skipt will convert the variable to it's specified type\n"
                         "\n"
                         "### Conditional Statements\n\n"
                         "Skipt is very direct with conditional statements, you can only have one condition in each statement. There are only 2 conditional statments, if, and while. For loops do not exist for the sole purpose of because a while loop can do the exact same thing, with some extra coding.\n"
                         "\n"
                         "**If-Statements**\n"
                         "\n"
                         "```\n"
                         "bool isAdult;\n"
                         "int age = 0;\n"
                         "if (age >= 18)\n"
                         "{\n"
                         "    isAdult = true\n"
                         "    > \"You are an adult!\"\n"
                         "}\n"
                         "else\n"
                         "{\n"
                         "    isAdult = false\n"
                         "    > \"You are not an adult!\"\n"
                         "}\n"
                         "\n"
                         "```\n"
                         "With If-Statements, there is one key formatting you must keep in mind. There must be a starting bracket the line after the conditional statement is defined. Here are a few examples:\n"
                         "```\n"
                         "# Valid\n"
                         "if (x == y)\n"
                         "{\n"
                         "    # Do something\n"
                         "}\n"
                         "\n"
                         "# Invalid\n"
                         "if (x == y){\n"
                         "    # [Error] | [Compile.cpp] [Starting Conditional Statement Bracket]: Conditional Start Statement Bracket is not expected on this line!\n"
                         "}\n"
                         "\n"
                         "# Invalid\n"
                         "if (x == y)\n"
                         "    # This will result in nothing happening, the program may unexpectedly quit\n"
                         "```\n"
                         "\n"
                         "**While-Loops**\n"
                         "```\n"
                         "int i = 0\n"
                         "while (i < 10)\n"
                         "{\n"
                         "    > i\n"
                         "    > \", \"\n"
                         "    \n"
                         "    i = i + 1\n"
                         "}\n"
                         "\n"
                         "# Output:\n"
                         "# 0, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000,\n"
                         "```\n"
                         "\n"
                         "*As seen above, even though we defined i as an integer, it displayed as if it was a double. This is because of how Skipt calcuates and stores variables. When a variable is defined as an integer, the program will register it as an integer, but in calculations, and processes, it is calculated as if it's a double. This is done to reduce the amount of conversions that the compiler has to do from one data-type to another.*";

        }

    }
};

#endif //SKIPT_DEFAULTFILES_H
