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
#include <string>
#include <cstring>
#include <unordered_map>


void Compile::Run(std::string fileName, bool asReference) {
    std::vector<std::string> fileContents = FileReader::Read(fileName);
    bool inConditional = false;
    int inConditionalStarts = 0;

    bool whileStatement = false;
    int whileStatementStarts = 0;

    std::cout << fileName << " => Length: " << fileContents.size();
    for (int i = 0; i < fileContents.size(); i++){
        if (fileContents[i][0] == '#'){
            continue;
        }


        if (fileContents[i].length() == 0){
            continue;
        }


        std::string line = fileContents[i];

        if (isblank(fileContents[i][0]) && inConditional){ // We dont need to strip the line if there's nothing to strip
            line = String::Strip(line); // Holy fuck,  this is critical to having the conditional statements work
        }

        if (String::Contains(line, ":") && String::Split(line, ":")[0].length() <= 2 && !String::Contains(line, "::")){ // We're defining a variable
            std::string tokenInitalizer;
            std::string tokenValue;
            std::string equalsValue;

            std::istringstream iss(line);
            if (iss >> tokenInitalizer >> equalsValue >> tokenValue){
                std::vector<std::string> tokenVector = String::Split(tokenInitalizer, ":");
                char type = tokenVector[0][0];

                if (Token::tokenExists(tokenVector[1])){ // Recast Variable to a new type
                    std::cout << "\n[DEBUG MODE] [Compile.cpp] : Preparing to Recast Variable : " << tokenVector[1] << "\n";

                    switch (type){
                        case 'i': // Recast to type integer
                            for (int i = 0; i < tokenValue.length(); i++){
                                char x = tokenValue[i];
                                if (isalpha(x)){
                                    std::cout << "Line (" << i + 1 << ") contains an error: " << tokenValue << " cannot be casted to type integer as it contains an alphabetical character!\n";
                                    exit(1);
                                }
                            }

                            if (tokenValue[0] == '*') { // Store the previous variable
                                Token temp = Token::getAllTokens(tokenVector[1]);
                                Token::recastVariable(tokenVector[1], Token::dataTypes::t_integer, temp.value);
                                std::cout << "\n[DEBUG MODE] [Compile.cpp] : Recasting Variable with previous data value\n";
                            }
                            else{
                                Token::recastVariable(tokenVector[1], Token::dataTypes::t_integer, tokenValue);
                            }

                            break;
                        case 'd':
                            for (int i = 0; i < tokenValue.length(); i++){
                                char x = tokenValue[i];
                                if (isalpha(x)){
                                    std::cout << "[Error] | [Compile.cpp] [Recast Variable]: " << tokenValue << " cannot be casted to type double as it contains an alphabetical character\n";
                                    std::cout << "        |> " << line << "\n";
                                    exit(1);
                                }
                            }

                            if (tokenValue[0] == '*') { // Store the previous variable
                                Token temp = Token::getAllTokens(tokenVector[1]);
                                Token::recastVariable(tokenVector[1], Token::dataTypes::t_double, temp.value);
                            }
                            else{
                                Token::recastVariable(tokenVector[1], Token::dataTypes::t_double, tokenValue);
                            }

                            break;

                    }
                }
                else{
                    std::cout << "\n[DEBUG MODE] [Compile.cpp] : Preparing to define variable\n";

                    switch (type){
                        case 'i': { // type integer
                            if (!String::IsInteger(tokenValue)){
                                std::cout << "Line (" << i + 1 << ") contains an error: " << tokenValue << " is not of type integer!\n";
                                std::cout << "Reference Line: " << line << "\n";
                                exit(1);
                            }
                            if (inConditional){
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_integer, true);
                            } else{
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_integer, false);
                            }

                            if (asReference)
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_integer, true);

                            break;
                        }
                        case 'd':{ // type double
                            if (!String::IsDouble(tokenValue)){
                                std::cout << "Line (" << i + 1 << ") contains an error: " << tokenValue << " is not of type double!\n";
                                std::cout << "Reference Line: " << line << "\n";
                                exit(1);
                            }

                            if (inConditional){
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_double, true);
                            } else{
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_double, false);
                            }
                            if (asReference)
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_double, true);


                            break;
                        }
                        case 's': { // type string
                            std::string parsedString = parseString(line, i);
                            if (inConditional) {
                                Token::DefineVariable(tokenVector[1], parsedString, Token::t_string, true);
                            } else {
                                Token::DefineVariable(tokenVector[1], parsedString, Token::t_string, false);
                            }

                            if (asReference)
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_string, true);
                            break;
                        }
                        case 'a':{ // Prepare to define a type of array, the compiler is unsure of what specific type
                            char specificType = tokenVector[0][1]; // Get the type of variable, from the second character in the data type
                            Token::dataTypes arrayType = Token::getType(specificType);

                            std::vector<std::string> getArrayValues = parseArray(tokenValue, i, arrayType); // get values of the array
                            std::string valueForToken = String::Join(getArrayValues, ",");
                            valueForToken = valueForToken.substr(0, valueForToken.size() - 1);

                            int arrayOffset = (Token::dataTypes)arrayType + 5;
                            if (inConditional)
                                Token::DefineVariable(tokenVector[1], valueForToken, (Token::dataTypes)arrayOffset, true);
                            else
                                Token::DefineVariable(tokenVector[1], valueForToken, (Token::dataTypes)arrayOffset, false);

                            if (asReference)
                                Token::DefineVariable(tokenVector[1], tokenValue, (Token::dataTypes)arrayOffset, true);

                            break;
                        }
                        case 'b':{
                            // Full fucking faith that you input a valid data-type
                            if (inConditional) {
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_boolean, true);
                            } else {
                                Token::DefineVariable(tokenVector[1], tokenValue, Token::t_boolean, false);
                            }
                            break;
                        }
                    }
                }
            }

            if (iss.fail()){
                std::cout << "[Error] | [Compile.cpp] [Modifying Variable] : Variable does not contain any value!\n";
                std::cout << "        |> " << line << "\n";
                exit(1);
            }
        }

        // Modify Variable
        if (Token::tokenExists(String::Split(line, " = ")[0]) && !String::Contains(line, ":")){

            std::cout << "\n[DEBUG MODE] [Compile.cpp] : Modifying Variable Value\n";

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

                std::cout << "\n[DEBUG MODE] [Compile.cpp] : Expression Parser for equation " << str << "\n";
                char* char_array = new char[str.length() + 1];
                std::strcpy(char_array, str.c_str());

                Parser p(char_array);
                Expression *expression = p.statement();

                if (expression){
                    Token::modifyToken(token, std::to_string(expression->evaluate()));
                    delete expression;
                }
                else{
                    std::cout << "Unable to compute expression" << "\n";
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
                std::cout << ">> " << parseString(line, i);
            }
            else{
                if (inConditional){
                    Token token = Token::getAllTokens(String::Split(line, " ")[1]);
                    std::cout << ">> " << token.value;
                }
                else{
                    Token token = Token::getToken(String::Split(line, " ")[1]);
                    std::cout << ">> " << token.value;
                }

            }
        }

        if (line[0] == '<'){ // Input
            Token token = Token::getToken(String::Split(line, " ")[1]);

            std::string input;
            std::getline(std::cin, input);

            Token::modifyToken(token, input);
        }

        if (String::Contains(line, "if")){

            std::cout << "\n[DEBUG MODE] [Compile.cpp] : Preparing for Conditional Statement of If Statement\n";

            std::string expression = "";
            size_t firstInstance = line.find('(');
            size_t secondInstance = line.find(')');
            firstInstance++;
            expression = line.substr(firstInstance, (secondInstance - firstInstance));

            std::istringstream expressionStringStream(expression);

            std::string variableOne;
            std::string _operator;
            std::string variableTwo;

            if (expressionStringStream >> variableOne >> _operator >> variableTwo){
                Token tokenOne;
                Token tokenTwo;

                if (!Token::tokenExists(variableOne) && !Token::tokenExists(variableTwo)){
                    std::cout << "[Error] | [Compile.cpp] [Conditional Statement]: Conditional operator does not contain a consistent token!\n";
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

                    std::cout << "\n[DEBUG MODE] [Compile.cpp] : " << tempVariableTwo.name << " has been created as a placeholder token for the conditional statement\n";
                }

                if (Token::tokenExists(variableTwo)){
                    tokenTwo = Token::getToken(variableTwo);
                }
                else{
                    tokenTwo.name = "conditionalVariable_2"; tokenTwo.dataType = tokenOne.dataType; tokenTwo.value = variableTwo;
                }

                Operator::Operators parsedOperator = Operator::ParseOperator(_operator);

                std::cout << "\n[DEBUG MODE] [Compile.cpp] : Parsing Operator for Conditional Statement\n";


                if (Operator::Condition(tokenOne, tokenTwo, parsedOperator)){
                    std::cout << "\n[DEBUG MODE] [Compile.cpp] : Conditional Statement has returned true\n";

                    inConditional = true;
                    inConditionalStarts = i + 1;
                }


            }
        }

        if (String::Contains(line, "ref")){ // NIGHTMARE NIGHTMARE NIGHTMARE
            std::string refVariableName = String::Split(line, " ")[1];
            Token returnToken = Functions::HandleCallFunction(String::Split(line, " ")[1]);
            if (String::Contains(line, ">>") && returnToken.dataType != Token::t_empty){
                std::string getTokenName = String::Split(line, ">>")[1];
                getTokenName = String::Strip(getTokenName);
                Token::modifyToken(Token::getToken(getTokenName), returnToken.value);
            }
            else if (String::Contains(line, ">>") && returnToken.dataType == Token::t_empty){
                std::cout << "[Error] | [Compile.cpp] [Reference Handler]: There is no return value for " << refVariableName << "\n";
                std::cout << "        |> " << line << "\n";
                exit(1);
            }
        }

        if (line[0] == '{'){
            if (inConditionalStarts != i && inConditional){
                std::cout << "[Error] | [Compile.cpp] [Starting Conditional Statement Bracket]: Conditional Start Statement Bracket is not expected on this line!\n";
                std::cout << "        |> " << line << "\n";
                exit(1);
            }
        }

        if (String::Contains(line, "free")){
            std::istringstream iss(line);

            std::string op;
            std::string token;

            if (iss >> op >> token){
                if (op == "free"){
                    bool x = Token::DeleteToken(token);
                    if (!x){
                        std::cout << token << " was unable to be freed from memory\n";
                    }
                }
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

    for (int i = 0; i < parsedString.length(); i++){
        char c = parsedString[i];
        if (c == '\\'){ // Literal
            switch (parsedString[i + 1]){
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

std::vector<std::string> Compile::parseArray(std::string line, int i, Token::dataTypes expectedType){
    std::vector<std::string> vectorValues;

    i++;

    size_t firstInstance = line.find('[');
    if (firstInstance == std::string::npos){
        std::cout << "Line (" << i << ") contains an error: Array could not be defined!\n";
    }

    size_t lastInstance = line.find(']');
    if (lastInstance == std::string::npos){
        std::cout << "Line (" << i << ") contains an error: Array could not be defined!\n";
    }

    std::string parsedString = line.substr(firstInstance, (lastInstance - firstInstance));

    firstInstance++;
    if (String::Contains(parsedString, " ")){
        parsedString = String::Replace(parsedString, " ", "");
    }
    std::vector<std::string> values = String::Split(parsedString, ",");
    for (int i = 0; i < values.size(); i++){

        std::string strippedValue = String::Strip(values[i]);

        if (Token::ValidateType(strippedValue, expectedType)){
            vectorValues.push_back(strippedValue);
        }
        else{
            std::cout << "Line (" << i << ") contains an error: " << strippedValue << " is not of expected data type\n";
            exit(1);
        }
    }

    return vectorValues;


}

bool Compile::sameDataType(Token token, std::string value, int lineNumber) {
    switch (token.dataType){ // Datatype we're expecting
        case 'i':
            if (!String::IsInteger(value)){
                std::cout << "Line (" << lineNumber << ") contains an error: Variable " << token.name << " expected data type Integer\n";
                return false;
            }
            return true;
        case 'd':
            if (!String::IsDouble(value)){
                std::cout << "Line (" << lineNumber << ") contains an error: Variable " << token.name << " expected data type Double\n";
                return false;
            }
            return true;
    }
}



