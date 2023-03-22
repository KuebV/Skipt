//
// Created by bobwt on 2/10/2023.
//

#include "Compile.h"
#include "FileReader.h"
#include "../Extensions/String.h"
#include "ExpressionParser/Parser.h"
#include "ExpressionParser.h"
#include "../Compiler/ExpressionParser/Expression.h"
#include "Conditional/Operator.h"
#include "Function/Functions.h"
#include "../Property/PropertyReference.h"
#include "../Property/PropertyFile.h"
#include "ExitMessage.h"
#include <string>
#include <cstring>
#include <unordered_map>
#include <chrono>

Compile::VariableTypes GetVariableTypes(std::string const& str){
    const std::unordered_map<std::string, Compile::VariableTypes> functionTable{
            { "int", Compile::VariableTypes::Integer},
            { "double", Compile::VariableTypes::Double},
            { "float", Compile::VariableTypes::Float},
            { "string", Compile::VariableTypes::String},
            { "bool", Compile::VariableTypes::Boolean},
            { "int[]", Compile::VariableTypes::IntegerArray},
            { "double[]", Compile::VariableTypes::DoubleArray},
            { "float[]", Compile::VariableTypes::FloatArray},
            { "string[]", Compile::VariableTypes::StringArray},
            { "bool[]", Compile::VariableTypes::BooleanArray},
            {"void", Compile::VariableTypes::Void},
            {"ref", Compile::VariableTypes::Reference},
            {"free*", Compile::VariableTypes::UnsafeFree},
            {"free", Compile::VariableTypes::Free},
            {"if", Compile::VariableTypes::If},
            {"while", Compile::VariableTypes::While}

    };

    auto it = functionTable.find(str);
    if (it != functionTable.end()) {
        return it->second;
    }
    else {
        return Compile::VariableTypes::Void;
    }
}


int Compile::i = 0;
void Compile::Run(std::string fileName, bool asReference) {
    PropertyReference propertyReference = PropertyFile::ReadPropertyFile("compiler.properties");
    ExitMessage exitMsg = ExitMessage("Compiler.cpp");

    std::vector<std::string> fileContents = FileReader::Read(fileName);
    bool inConditional = false;
    int inConditionalStarts = 0;

    bool whileStatement = false;
    int whileStatementStarts = 0;

    std::cout << fileName << " => Length: " << fileContents.size() << "\n";

    for (i = 0; i < fileContents.size(); i++){
        if (fileContents[i][0] == '#'){
            continue;
        }
        if (fileContents[i].length() == 0){
            continue;
        }

        std::string line = fileContents[i];

        if (isblank(fileContents[i][0]) && inConditional){ // We dont need to strip the line if there's nothing to strip
            line = String::Strip(line); // Holy fuck, this is critical to having the conditional statements work
        }

        if (line.at(line.length() - 1) == ';') // Yet another string serialization
            line.erase(line.length() - 1, 1);

        std::vector<std::string> lineElements = String::Split(line, " ");

        switch (GetVariableTypes(lineElements[0])){ // Defining a new variable
            case Integer:{
                if (Token::tokenExists(lineElements[1])){
                    exitMsg.Error("Define Integer", lineElements[1] + " has already been defined as a variable!", line, 1);
                }

                if (!String::Contains(line, "=")){
                    Token::DefineVariable(lineElements[1], "0", Token::t_integer, inConditional);
                    break;
                }

                std::string variableValue = String::Strip(String::Split(line, "=")[1]);
                if (!String::IsInteger(variableValue)){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type integer\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                Token::DefineVariable(lineElements[1], variableValue, Token::t_integer, inConditional);
                break;
            }
            case String: {
                if (Token::tokenExists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (!String::Contains(line, "=")){
                    Token::DefineVariable(lineElements[1], "", Token::t_string, inConditional);
                    break;
                }

                std::string variableValue = String::Strip(String::Split(line, "=")[1]);

                if (!String::Contains(variableValue, "\"")){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type string\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }
                variableValue = parseString(variableValue, i);

                Token::DefineVariable(lineElements[1], variableValue, Token::t_string, inConditional);

                break;
            }
            case Double:{
                if (Token::tokenExists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (!String::Contains(line, "=")){
                    Token::DefineVariable(lineElements[1], "0.0", Token::t_double, inConditional);
                    break;
                }

                std::string variableValue = String::Strip(String::Split(line, "=")[1]);
                if (!String::IsDouble(variableValue)){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type double\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                Token::DefineVariable(lineElements[1], variableValue, Token::t_double, inConditional);
                break;
            }
            case Boolean:{
                if (Token::tokenExists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (!String::Contains(line, "=")){
                    Token::DefineVariable(lineElements[1], "false", Token::t_boolean, inConditional);
                    break;
                }

                std::string variableValue = String::Strip(String::Split(line, "=")[1]);
                Token::DefineVariable(lineElements[1], variableValue, Token::t_boolean, inConditional);
                break;
            }
            case IntegerArray:{
                if (Token::tokenExists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (!String::Contains(line, "=")){
                    Token integerArrayToken;
                    integerArrayToken.name = lineElements[1];
                    integerArrayToken.dataType = Token::t_intArray;

                    Token::DefineVariable(integerArrayToken);

                    break;
                }

                std::string intArraySubstring = String::Substring(String::Split(line, "=")[1], "{", "}");
                std::vector<std::string> intArrayElements = String::Split(intArraySubstring, ",");
                for (auto & intArrayElement : intArrayElements){
                    intArrayElement = String::Strip(intArrayElement);
                }
                Token::DefineVariable(lineElements[1], intArraySubstring, Token::t_intArray, inConditional);

                for (int j = 0; j < intArrayElements.size(); j++){
                    std::string variableName = lineElements[1] + "[" + std::to_string(j) + "]";
                    std::string variableValue = intArrayElements[j];

                    Token::DefineVariable(variableName, variableValue, Token::t_integer, inConditional);
                }
                break;
            }
            case Reference:{
                std::string refVariableName = String::Split(line, " ")[1];

                std::string functionCall = "";

                if (String::Contains(line, ">>")){
                    functionCall = String::Substring(line, " ", ">>");
                }
                else{
                    size_t spaceIndex = String::IndexOf(line, " ");
                    size_t endIndex = line.length();

                    functionCall = line.substr(spaceIndex, endIndex);
                    functionCall = String::Strip(functionCall);
                }

                Token returnToken = Functions::HandleCallFunction(functionCall);
                if (String::Contains(line, ">>") && returnToken.dataType != Token::t_empty){
                    std::string getTokenName = String::Split(line, ">>")[1];
                    getTokenName = String::Strip(getTokenName);

                    Token getToken = Token::getAllTokens(getTokenName);
                    if (getToken.dataType != returnToken.dataType){
                        std::cout << "[Error] | [Compile.cpp] [Reference Handler]: Return Variable and Defined Variable are not the same data-type!\n";
                        std::cout << "        |> " << line << "\n";
                        std::cout << "[Context for Above]:\nReturn Variable: " << returnToken.dataType << "\nDefined Variable: " << getToken.dataType << "\n";
                        exit(1);
                    }
                    Token::modifyToken(getToken, returnToken.value);
                }
                else if (String::Contains(line, ">>") && returnToken.dataType == Token::t_empty){
                    std::cout << "[Error] | [Compile.cpp] [Reference Handler]: There is no return value for " << refVariableName << "\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }
                break;
            }
            case Free:{
                std::string freeToken = line.substr(String::IndexOf(line, " ") + 1, (line.length() - String::IndexOf(line, " ")));
                freeToken = String::Strip(freeToken);
                if (String::Contains(freeToken, "[") && String::Contains(freeToken, "]")){
                    exitMsg.Error("Compile::Run", "Cannot free an element of an array", line, 1);
                }
                if (Token::getAllTokens(freeToken).dataType == Token::dataTypes::t_intArray ||
                    Token::getAllTokens(freeToken).dataType == Token::dataTypes::t_strArray ||
                    Token::getAllTokens(freeToken).dataType == Token::dataTypes::t_floatArray ||
                    Token::getAllTokens(freeToken).dataType == Token::dataTypes::t_doubleArray){

                    Token t = Token::getAllTokens(freeToken);
                    int totalTokens = String::Split(t.value, ",").size();
                    for (int j = 0; j < totalTokens; j++){
                        std::string *x = new std::string();
                        *x = t.name + "[" + std::to_string(j) + "]";
                        Token::DeleteToken(*x);
                        free(x);
                    }
                }
            }
            case UnsafeFree:{
                // In the case of arrays, the unsafe array will delete the element without resizing, or checking the parent array
                // It will unsafely, delete the token entirely
                std::string freeToken = line.substr(String::IndexOf(line, " ") + 1, (line.length() - String::IndexOf(line, " ")));
                freeToken = String::Strip(freeToken);

                if (!Token::tokenExists(freeToken)){
                    std::cout << "[Error] | [Compile.cpp] [Freeing Variables]: Token was unable to be freed, because it does not exist!\n";
                    std::cout << "        |> " << freeToken << "\n";
                    exit(1);
                }
                Token t = Token::getToken(freeToken);
                bool x = Token::DeleteToken(freeToken);
                if (!x){
                    std::cout << freeToken << " was unable to be freed from memory\n";
                }
                break;
            }
            case If:{
                std::string expression = String::Substring(line, "(", ")");
                std::istringstream expressionStringStream(expression);

                std::string variableOne;
                std::string _operator;
                std::string variableTwo;

                if (expressionStringStream >> variableOne >> _operator >> variableTwo){
                    Token tokenOne;
                    Token tokenTwo;

                    if (!Token::tokenExists(variableOne) && !Token::tokenExists(variableTwo)){
                        std::cout << "[Error] | [Compile.cpp] [Conditional Statement(If)]: Conditional operator does not contain a consistent token!\n";
                        std::cout << "        |> " << line << "\n";
                        exit(1);
                    }

                    if (Token::tokenExists(variableOne)){
                        tokenOne = Token::getToken(variableOne);
                    }
                    else{
                        Token tempVariableTwo;
                        tempVariableTwo = Token::getToken(variableTwo);
                        Token::dataTypes primitiveType = Token::GetPrimitiveType(tempVariableTwo.dataType);
                        tokenOne.name = "conditionalVariable_1"; tokenOne.dataType = primitiveType; tokenOne.value = variableOne;
                    }

                    if (Token::tokenExists(variableTwo)){
                        tokenTwo = Token::getToken(variableTwo);
                    }
                    else{
                        tokenTwo.name = "conditionalVariable_2"; tokenTwo.dataType = tokenOne.dataType; tokenTwo.value = variableTwo;
                    }

                    Operator::Operators parsedOperator = Operator::ParseOperator(_operator);

                    if (Operator::Condition(tokenOne, tokenTwo, parsedOperator)){
                        inConditional = true;
                        inConditionalStarts = i + 1;
                    }
                }
            }
            case While:{
                std::string expression = String::Substring(line, "(", ")");
                std::istringstream iss(expression);

                std::string variableOne, _operator, variableTwo;
                if (iss >> variableOne >> _operator >> variableTwo){
                    Token tokenOne;
                    Token tokenTwo;

                    if (!Token::tokenExists(variableOne) && !Token::tokenExists(variableTwo)){
                        std::cout << "[Error] | [Compile.cpp] [Conditional Statement(While)]: Conditional operator does not contain a consistent token!\n";
                        std::cout << "        |> " << line << "\n";
                        exit(1);
                    }

                    // This is exactly what the conditional statement for if does
                    if (Token::tokenExists(variableOne)){
                        tokenOne = Token::getToken(variableOne);
                    }
                    else{
                        Token tempVariableTwo;
                        tempVariableTwo = Token::getToken(variableTwo);
                        Token::dataTypes primitiveType = Token::GetPrimitiveType(tempVariableTwo.dataType);
                        tokenOne.name = "conditionalVariable_1"; tokenOne.dataType = primitiveType; tokenOne.value = variableOne;
                    }

                    if (Token::tokenExists(variableTwo)){
                        tokenTwo = Token::getToken(variableTwo);
                    }
                    else{
                        tokenTwo.name = "conditionalVariable_2"; tokenTwo.dataType = tokenOne.dataType; tokenTwo.value = variableTwo;
                    }

                    Operator::Operators parsedOperator = Operator::ParseOperator(_operator);

                    if (Operator::Condition(tokenOne, tokenTwo, parsedOperator)){
                        inConditional = true;
                        inConditionalStarts = i + 1;
                        whileStatement = true;
                        whileStatementStarts = i - 1;
                    }
                    else{ // Whoops! Not having this causing an infinite loop!
                        inConditional = false;
                        whileStatement = false;
                    }

                }
            }
            default:
                break;
        }



        // Modify Variable
        if (Token::tokenExists(String::Split(line, " = ")[0]) && !String::Contains(line, ":")){
            // Get the token that is being modified
            Token token = Token::getToken(String::Split(line, " = ")[0]);

            if (token.dataType == Token::t_string){
                std::string str = parseString(line, i);
                Token::modifyToken(token, str);
            }

            int lineLength = line.length();
            size_t equals = line.find('=');
            std::string str = line.substr(equals + 1, lineLength - 1);

            str = ExpressionParser::ReplaceVariableNames(str);
            str = String::Strip(str);

            if (String::Split(str, " ").size() <= 1){
                Token::modifyToken(token, str);
            }


            // Expression Parser
            if (String::Contains(line, "+") || String::Contains(line, "-") || String::Contains(line, "*") || String::Contains(line, "/")){
                int lineLength = line.length();
                size_t equals = line.find('=');
                std::string str = line.substr(equals + 1, lineLength - 1);

                str = ExpressionParser::ReplaceVariableNames(str);
                char* char_array = new char[str.length() + 1];
                std::strcpy(char_array, str.c_str());

                Parser p(char_array);
                Expression *expression = p.statement();

                if (expression){
                    Token::modifyToken(token, std::to_string(expression->evaluate()));
                    delete expression;
                }
                else{
                    std::cout << "[Error] | [Compile.cpp] [Expression Parser]: Unable to compute expression!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }
            }
        }

        if (line[0] == '>'){ // Output
            if (line.length() < 3){
                std::cout << "\n";
                continue;
            }


            if (String::Contains(line, "\"")){
                std::cout << parseString(line, i);
            }
            else{
                if (inConditional){
                    Token token = Token::getAllTokens(String::Split(line, " ")[1]);
                    std::cout << token.value;
                }
                else{
                    Token token = Token::getToken(String::Split(line, " ")[1]);
                    std::cout << token.value;
                }

            }
        }

        if (line[0] == '<'){ // Input
            Token token = Token::getToken(String::Split(line, " ")[1]);

            std::string input;
            std::getline(std::cin, input);

            Token::modifyToken(token, input);
        }



        if (line[0] == '{'){
            if (inConditionalStarts != i && inConditional){
                std::cout << "[Error] | [Compile.cpp] [Starting Conditional Statement Bracket]: Conditional Start Statement Bracket is not expected on this line!\n";
                std::cout << "        |> " << line << "\n";
                exit(1);
            }
        }

        // Part of the While-Statement Condition
        if ((i + 1) <= fileContents.size()){
            try{
                if (fileContents.at(i + 1)[0] == '}' && whileStatement){ // Peek ahead
                    i = whileStatementStarts;
                }
            }
            catch (std::out_of_range orr){
                continue;
            }
        }

        if (line[0] == '}'){
            if (String::Contains(fileContents[i + 1], "else") && !inConditional){
                inConditional = true;
                inConditionalStarts = i + 2;
            }
            else{
                inConditional = false;
                Token::ConditionalTokenMap.clear();
            }
        }
    }
}

std::string Compile::parseString(std::string line, int lineNumber) {
    std::string parsedString;

    lineNumber++;

    size_t firstInstance = line.find('"');
    if (firstInstance == std::string::npos){
        std::cout << "Line (" << lineNumber << ") contains an error: String could not be parsed properly!\n";
        exit(1);
    }


    size_t lastInstance = line.find_last_of('"');
    if (lastInstance == std::string::npos){
        std::cout << "Line (" << lineNumber << ") contains an error: String could not be parsed properly!\n";
        exit(1);
    }

    firstInstance++;
    parsedString = line.substr(firstInstance, (lastInstance - firstInstance));

    for (int j = 0; j < parsedString.length(); j++){
        char c = parsedString[j];
        if (c == '\\'){ // Literal
            switch (parsedString[j + 1]){
                case 'n':

                    // Jesus christ, I'm stupid. Parsed String wasn't being set to the newly replaced string.
                    // Possibly change String::Replace to contain the & modify operator?
                    parsedString = String::Replace(parsedString, "\\n", "\n");
                    break;
                default:
                    break;
            }
        }

    }
    return parsedString;
}

