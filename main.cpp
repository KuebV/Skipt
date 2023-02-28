#include <iostream>
#include "Compiler/Token.h"
#include "Compiler/DefaultFiles.h"
#include "Extensions/String.h"
#include "Compiler/Compile.h"

static PropertyReference propRef;
static const std::string CompilerPropertiesFile = "compiler.properties";
static Compile compiler;

int main() {

    std::cout << "Attempting to read " << CompilerPropertiesFile << "\n";

    // Compiler Properties:
    // FileToCompile => string => File that is read
    // GenerateNewFile => boolean => If the file is not found, should it be generated?
    if (!PropertyFile::fileExists(CompilerPropertiesFile)){
        std::cout << CompilerPropertiesFile << " being written\n";
        DefaultFiles::WriteConfigurationFile();
    }

    propRef = PropertyFile::ReadPropertyFile(CompilerPropertiesFile);
    if (!PropertyFile::fileExists(propRef.getValue("FileToCompile"))){
        std::cout << "From " << CompilerPropertiesFile << ": \"" << propRef.getValue("FileToCompile") << "\" was not found!\n";
        if (String::ToBoolean(propRef.getValue("GenerateNewFile"))){
            std::cout << "From " << CompilerPropertiesFile << ": Generating " << propRef.getValue("FileToCompile") << "\n";
            DefaultFiles::WriteDefaultGeneratedFile(propRef.getValue("FileToCompile"));
        }
    }

    std::cout << CompilerPropertiesFile << " has been read, and stored\n";

    std::cout << "Reading Skipt File\n";
    compiler.Run(propRef.getValue("FileToCompile"), false);

}
