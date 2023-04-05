#include <iostream>
#include <chrono>
#include "Compiler/Variable.h"
#include "Compiler/DefaultFiles.h"
#include "Extensions/StringExt.h"
#include "Compiler/Compile.h"
#include "Compiler/Function/Functions.h"
#include "ModifiedType/String.h"

static PropertyReference propRef;
static const std::string CompilerPropertiesFile = "compiler.properties";
static Compile compiler;

int main() {

    std::cout << ">--- >> Skipt << ---<\n";
    std::cout << "[PRE-COMPILER] Attempting to read " << CompilerPropertiesFile << "\n";

    // Compiler Properties:
    // FileToCompile => string => File that is read
    // GenerateNewFile => boolean => If the file is not found, should it be generated?
    if (!PropertyFile::fileExists(CompilerPropertiesFile)){
        std::cout << CompilerPropertiesFile << " being written\n";
        DefaultFiles::WriteConfigurationFile();
    }

    propRef = PropertyFile::ReadPropertyFile(CompilerPropertiesFile);
    if (!PropertyFile::fileExists(propRef.getValue("FileToCompile"))){
        std::cout << "[PRE-COMPILER]  From " << CompilerPropertiesFile << ": \"" << propRef.getValue("FileToCompile") << "\" was not found!\n";
        if (StringExt::ToBoolean(propRef.getValue("GenerateNewFile"))){
            std::cout << "[PRE-COMPILER]   From " << CompilerPropertiesFile << ": Generating " << propRef.getValue("FileToCompile") << "\n";
            DefaultFiles::WriteDefaultGeneratedFile(propRef.getValue("FileToCompile"));
        }
    }

    std::cout << "[PRE-COMPILER] " << CompilerPropertiesFile << " has been read, and stored\n";

    std::cout << propRef.getValue("FileToCompile")  << " is now being executed, view below for potential output:\n-------------------------\n";

    bool executionTime = StringExt::ToBoolean(propRef.getValue("executionTime"));
    if (executionTime){
        auto stopwatchStart = std::chrono::high_resolution_clock::now();
        compiler.Run(propRef.getValue("FileToCompile"), false);
        auto stopwatchEnd = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stopwatchEnd - stopwatchStart);

        std::cout << "\nFinished Executing in " << std::to_string(microseconds.count() / 1000) << "ms\n";
    }
    else{
        compiler.Run(propRef.getValue("FileToCompile"), false);
    }


    if (StringExt::ToBoolean(propRef.getValue("keepOpen"))){
        int x = 0;
        std::cin >> x;
        exit(x);
    }


}
