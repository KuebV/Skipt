//
// Created by bobwt on 2/10/2023.
//

#include "Compile.h"
Compile::VariableTypes GetVariableTypes(std::string const& str){
    const std::unordered_map<std::string, Compile::VariableTypes> functionTable{
            { "int", Compile::VariableTypes::Integer},
            { "double", Compile::VariableTypes::Double},
            { "float", Compile::VariableTypes::Float},
            { "string", Compile::VariableTypes::StringType},
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

void Compile::Run(std::string fileName, bool asExternal) {
    PropertyReference propertyReference = PropertyFile::ReadPropertyFile("compiler.properties");
    ExitMessage exitMsg = ExitMessage("Compiler.cpp");

    std::string fileNameModified = fileName.substr(0, StringExt::IndexOf(fileName, "."));

    std::vector<std::string> fileContents = FileReader::Read(fileName);
    bool inConditional = false;
    int inConditionalStarts = 0;

    bool whileStatement = false;
    int whileStatementStarts = 0;

    std::cout << fileName << " => Length: " << fileContents.size() << "\n";

    for (int i = 0; i < fileContents.size(); i++){
        if (fileContents[i][0] == '#'){
            continue;
        }
        if (fileContents[i].length() == 0){
            continue;
        }

        std::string line = fileContents[i];

        if (isblank(fileContents[i][0]) && inConditional){ // We dont need to strip the line if there's nothing to strip
            line = StringExt::Strip(line); // Holy fuck, this is critical to having the conditional statements work
        }

        if (line.at(line.length() - 1) == ';') // Yet another string serialization
            line.erase(line.length() - 1, 1);

        std::vector<std::string> lineElements = StringExt::Split(line, " ");

        switch (GetVariableTypes(lineElements[0])){ // Defining a new variable
            case Integer:{
                if (Variable::Exists(lineElements[1])){
                    exitMsg.Error("Define Integer", lineElements[1] + " has already been defined as a variable!", line, 1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable::DefineVariable(lineElements[1], "0", Variable::t_integer, inConditional);
                    break;
                }

                std::string variableValue = StringExt::Strip(StringExt::Split(line, "=")[1]);
                if (!StringExt::IsInteger(variableValue)){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type integer\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                Variable::DefineVariable(lineElements[1], variableValue, Variable::t_integer, inConditional);
                continue;
            }
            case StringType: {
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable::DefineVariable(lineElements[1], "", Variable::t_string, inConditional);
                    break;
                }

                std::string variableValue = StringExt::Strip(StringExt::Split(line, "=")[1]);

                if (!StringExt::Contains(variableValue, "\"")){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type string\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }
                variableValue = parseString(variableValue, i);

                Variable::DefineVariable(lineElements[1], variableValue, Variable::t_string, inConditional);

                continue;
            }
            case Double:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable::DefineVariable(lineElements[1], "0.0", Variable::t_double, inConditional);
                    break;
                }


                std::string variableValue = StringExt::Strip(StringExt::Split(line, "=")[1]);
                if (!StringExt::IsDouble(variableValue)){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type double\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                Variable::DefineVariable(lineElements[1], variableValue, Variable::t_double, inConditional);
                continue;
            }
            case Float:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable::DefineVariable(lineElements[1], "0f", Variable::t_float, inConditional);
                    break;
                }

                std::string variableValue = StringExt::Strip(StringExt::Split(line, "=")[1]);
                if (!StringExt::IsDouble(variableValue)){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << variableValue << " is not of type float\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                Variable::DefineVariable(lineElements[1], variableValue, Variable::t_floatArray, inConditional);
                continue;
            }
            case Boolean:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable::DefineVariable(lineElements[1], "false", Variable::t_boolean, inConditional);
                    break;
                }

                std::string variableValue = StringExt::Strip(StringExt::Split(line, "=")[1]);
                Variable::DefineVariable(lineElements[1], variableValue, Variable::t_boolean, inConditional);
                continue;
            }
            case IntegerArray:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (asExternal)
                    lineElements[1] = fileNameModified + "[" + lineElements[1] + "]";

                if (!StringExt::Contains(line, "=")){
                    Variable integerArrayToken;
                    integerArrayToken.name = lineElements[1];
                    integerArrayToken.dataType = Variable::t_intArray;

                    Variable::DefineVariable(integerArrayToken);

                    break;
                }

                std::string intArraySubstring = StringExt::Substring(StringExt::Split(line, "=")[1], "{", "}");
                std::vector<std::string> intArrayElements = StringExt::Split(intArraySubstring, ",");
                for (auto & intArrayElement : intArrayElements){
                    intArrayElement = StringExt::Strip(intArrayElement);
                }
                Variable::DefineVariable(lineElements[1], intArraySubstring, Variable::t_intArray, inConditional);

                for (int j = 0; j < intArrayElements.size(); j++){
                    std::string variableName = lineElements[1] + "[" + std::to_string(j) + "]";
                    std::string variableValue = intArrayElements[j];

                    Variable::DefineVariable(variableName, variableValue, Variable::t_integer, inConditional);
                }
                continue;
            }
            case DoubleArray:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (!StringExt::Contains(line, "=")){
                    Variable arrayToken;
                    arrayToken.name = lineElements[1];
                    arrayToken.dataType = Variable::t_doubleArray;

                    Variable::DefineVariable(arrayToken);

                    break;
                }

                std::string arraySubstring = StringExt::Substring(StringExt::Split(line, "=")[1], "{", "}");
                std::vector<std::string> arrayElements = StringExt::Split(arraySubstring, ",");
                for (auto & intArrayElement : arrayElements){
                    intArrayElement = StringExt::Strip(intArrayElement);
                }
                Variable::DefineVariable(lineElements[1], arraySubstring, Variable::t_doubleArray, inConditional);

                for (int j = 0; j < arrayElements.size(); j++){
                    std::string variableName = lineElements[1] + "[" + std::to_string(j) + "]";
                    std::string variableValue = arrayElements[j];

                    Variable::DefineVariable(variableName, variableValue, Variable::t_double, inConditional);
                }
                continue;
            }
            case StringArray:{
                if (Variable::Exists(lineElements[1])){
                    std::cout << "[Error] | [Compile.cpp] [Define Variable]: " << lineElements[1] << " has already been defined as a variable!\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                if (std::isdigit(lineElements[1][0])){
                    exitMsg.Error("StringArray Definition", "Variable Name cannot start with a numeric digit!", line, 1);
                }

                if (!StringExt::Contains(line, "=")){
                    Variable arrayToken;
                    arrayToken.name = lineElements[1];
                    arrayToken.dataType = Variable::t_strArray;

                    Variable::DefineVariable(arrayToken);

                    break;
                }

                std::string arraySubstring = StringExt::Substring(StringExt::Split(line, "=")[1], "{", "}");
                std::vector<std::string> arrayElements = StringExt::Split(arraySubstring, ",");
                for (auto & intArrayElement : arrayElements){
                    intArrayElement = StringExt::Strip(intArrayElement);
                }
                Variable::DefineVariable(lineElements[1], arraySubstring, Variable::t_strArray, inConditional);

                for (int j = 0; j < arrayElements.size(); j++){
                    std::string variableName = lineElements[1] + "[" + std::to_string(j) + "]";
                    std::string variableValue = arrayElements[j];

                    variableValue = parseString(variableValue, i);

                    Variable::DefineVariable(variableName, variableValue, Variable::t_strArray, inConditional);
                }

                continue;
            }
            case Reference:{
                std::string refVariableName = StringExt::Split(line, " ")[1];

                std::string functionCall = "";

                if (StringExt::Contains(line, ">>")){
                    functionCall = StringExt::Substring(line, " ", ">>");
                }
                else{
                    size_t spaceIndex = StringExt::IndexOf(line, " ");
                    size_t endIndex = line.length();

                    functionCall = line.substr(spaceIndex, endIndex);
                    functionCall = StringExt::Strip(functionCall);
                }

                Variable returnToken = Functions::HandleCallFunction(functionCall);
                if (StringExt::Contains(line, ">>") && returnToken.dataType != Variable::t_empty){
                    std::string getTokenName = StringExt::Split(line, ">>")[1];
                    getTokenName = StringExt::Strip(getTokenName);

                    Variable getToken = Variable::Get(getTokenName);
                    if (getToken.dataType != returnToken.dataType){
                        std::cout << "[Error] | [Compile.cpp] [Reference Handler]: Return Variable and Defined Variable are not the same data-type!\n";
                        std::cout << "        |> " << line << "\n";
                        std::cout << "[Context for Above]:\nReturn Variable: " << Variable::EnumToString(returnToken.dataType) << "\nDefined Variable: " << Variable::EnumToString(getToken.dataType) << "\n";
                        exit(1);
                    }
                    Variable::modifyVariable(getToken, returnToken.value);
                }
                else if (StringExt::Contains(line, ">>") && returnToken.dataType == Variable::t_empty){
                    std::cout << "[Error] | [Compile.cpp] [Reference Handler]: There is no return value for " << refVariableName << "\n";
                    std::cout << "        |> " << line << "\n";
                    exit(1);
                }

                continue;
            }
            case Free:{
                std::string freeToken = line.substr(StringExt::IndexOf(line, " ") + 1, (line.length() - StringExt::IndexOf(line, " ")));
                freeToken = StringExt::Strip(freeToken);
                if (StringExt::Contains(freeToken, "[") && StringExt::Contains(freeToken, "]")){
                    exitMsg.Error("Compile::Run", "Cannot free an element of an array", line, 1);
                }
                if (Variable::Get(freeToken).dataType == Variable::dataTypes::t_intArray ||
                        Variable::Get(freeToken).dataType == Variable::dataTypes::t_strArray ||
                        Variable::Get(freeToken).dataType == Variable::dataTypes::t_floatArray ||
                        Variable::Get(freeToken).dataType == Variable::dataTypes::t_doubleArray){

                    Variable t = Variable::Get(freeToken);
                    int totalTokens = StringExt::Split(t.value, ",").size();
                    for (int j = 0; j < totalTokens; j++){
                        std::string *x = new std::string();
                        *x = t.name + "[" + std::to_string(j) + "]";
                        Variable::DeleteToken(*x);
                        free(x);
                    }
                }

                continue;
            }
            case UnsafeFree:{
                // In the case of arrays, the unsafe array will delete the element without resizing, or checking the parent array
                // It will unsafely, delete the token entirely
                std::string freeToken = line.substr(StringExt::IndexOf(line, " ") + 1, (line.length() - StringExt::IndexOf(line, " ")));
                freeToken = StringExt::Strip(freeToken);

                if (!Variable::Exists(freeToken)){
                    std::cout << "[Error] | [Compile.cpp] [Freeing Variables]: Variable was unable to be freed, because it does not exist!\n";
                    std::cout << "        |> " << freeToken << "\n";
                    exit(1);
                }
                Variable t = Variable::Get(freeToken);
                bool x = Variable::DeleteToken(freeToken);
                if (!x){
                    std::cout << freeToken << " was unable to be freed from memory\n";
                }

                continue;

            }
            case If:{
                std::string expression = StringExt::Substring(line, "(", ")");
                std::istringstream expressionStringStream(expression);

                std::string variableOne;
                std::string _operator;
                std::string variableTwo;

                if (expressionStringStream >> variableOne >> _operator >> variableTwo){
                    Variable tokenOne;
                    Variable tokenTwo;

                    if (!Variable::Exists(variableOne) && !Variable::Exists(variableTwo)){
                        std::cout << "[Error] | [Compile.cpp] [Conditional Statement(If)]: Conditional operator does not contain a consistent token!\n";
                        std::cout << "        |> " << line << "\n";
                        exit(1);
                    }

                    if (Variable::Exists(variableOne)){
                        tokenOne = Variable::Get(variableOne);
                    }
                    else{
                        Variable tempVariableTwo;
                        tempVariableTwo = Variable::Get(variableTwo);
                        Variable::dataTypes primitiveType = Variable::GetPrimitiveType(tempVariableTwo.dataType);
                        tokenOne.name = "conditionalVariable_1"; tokenOne.dataType = primitiveType; tokenOne.value = variableOne;
                    }

                    if (Variable::Exists(variableTwo)){
                        tokenTwo = Variable::Get(variableTwo);
                    }
                    else{
                        tokenTwo.name = "conditionalVariable_2"; tokenTwo.dataType = tokenOne.dataType; tokenTwo.value = variableTwo;
                    }

                    Operator::Operators parsedOperator = Operator::ParseOperator(_operator);

                    if (Operator::Condition(tokenOne, tokenTwo, parsedOperator)){
                        inConditional = true;
                        inConditionalStarts = i + 1;
                    }

                    continue;
                }
            }
            case While:{
                std::string expression = StringExt::Substring(line, "(", ")");
                std::istringstream iss(expression);

                std::string variableOne, _operator, variableTwo;
                if (iss >> variableOne >> _operator >> variableTwo){
                    Variable tokenOne;
                    Variable tokenTwo;

                    if (!Variable::Exists(variableOne) && !Variable::Exists(variableTwo)){
                        std::cout << "[Error] | [Compile.cpp] [Conditional Statement(While)]: Conditional operator does not contain a consistent token!\n";
                        std::cout << "        |> " << line << "\n";
                        exit(1);
                    }

                    // This is exactly what the conditional statement for if does
                    if (Variable::Exists(variableOne)){
                        tokenOne = Variable::Get(variableOne);
                    }
                    else{
                        Variable tempVariableTwo;
                        tempVariableTwo = Variable::Get(variableTwo);
                        Variable::dataTypes primitiveType = Variable::GetPrimitiveType(tempVariableTwo.dataType);
                        tokenOne.name = "conditionalVariable_1"; tokenOne.dataType = primitiveType; tokenOne.value = variableOne;
                    }

                    if (Variable::Exists(variableTwo)){
                        tokenTwo = Variable::Get(variableTwo);
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

                    continue;

                }
            }
            default:{
                break;
            }

        }

        /*std::string lexerString = "";
        for (int j = 0; j < line.length(); j++){
            if (std::isalpha(line[j]))
                continue;

            char currentCharacter = line[j];
            char nextCharacter;
            if (j < line.length() - 1)
                nextCharacter = line[j + 1];

            if (!Operator::IsOperator(currentCharacter, nextCharacter))
                continue;
            else{
                std::string operatorString; operatorString += currentCharacter; operatorString += nextCharacter;
                Operator::Operators _operator = Operator::ParseOperator(operatorString);
                if (Operator::RequireSecondVariable(_operator)){
                    std::string subString = line.substr(i + 5, (line.length() - i));
                    subString = StringExt::Strip(subString);


                    if (_operator == Operator::Operators::Increment){

                    }
                }
            }
        }*/


        String lineString(line);
        if (lineString.Contains('=')){

            if (lineString.Contains("+="))
                lineString.ptr_ReplaceMulti("+=", "=");

            if (lineString.Contains("-="))
                lineString.ptr_ReplaceMulti("-=", "=");

            std::unique_ptr<String> potentialVariable = std::make_unique<String>(lineString.Substring(0, lineString.Until('=')));
            *potentialVariable = potentialVariable->Strip(String::StripOptions::All);

            if (!Variable::Exists(potentialVariable->ToString())){
                exitMsg.Error("Lexer", "Variable does not exist!", line, 1);
            }

            Variable var = Variable::Get(potentialVariable->ToString());
            if (var.dataType == Variable::t_string){
                Variable::modifyVariable(var, lineString.ContentBetween("\"", "\"").ToString());
                break;
            }

            std::vector<std::string> expressionOperators = {"+", "-", "/", "*", "%", "(", ")"};
            if (lineString.ContainsAny(expressionOperators)){
                size_t startIndex = lineString.Until('=');
                std::string expression = lineString.Substring(startIndex + 1, (lineString.Length - startIndex)).ToString();
                expression = StringExt::Strip(expression);

                std::string expressionReplaced = ExpressionParser::ReplaceVariableNames(expression);

                char* char_array = new char[expressionReplaced.length() + 1];
                std::strcpy(char_array, expressionReplaced.c_str());

                Parser p(char_array);
                Expression *expressionParser = p.statement();

                if (expressionParser){
                    Variable::modifyVariable(var, std::to_string(expressionParser->evaluate()));
                    delete expressionParser;
                }
                else{
                    exitMsg.Error("Lexer.ExpressionParser", "Cannot Compute Expression, view expression below", expression, 1);
                }
            }
        }

        if (line[0] == '>'){ // Output
            if (line.length() < 3){
                std::cout << "\n";
                continue;
            }


            if (StringExt::Contains(line, "\"")){
                std::cout << parseString(line, i);
            }
            else{
                if (inConditional){
                    Variable token = Variable::Get(StringExt::Split(line, " ")[1]);
                    std::cout << token.value;
                }
                else{
                    Variable token = Variable::Get(StringExt::Split(line, " ")[1]);
                    std::cout << token.value;
                }

            }
        }

        if (line[0] == '<'){ // Input
            Variable token = Variable::Get(StringExt::Split(line, " ")[1]);

            std::string input;
            std::getline(std::cin, input);

            Variable::modifyVariable(token, input);
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
            if (StringExt::Contains(fileContents[i + 1], "else") && !inConditional){
                inConditional = true;
                inConditionalStarts = i + 2;
            }
            else{
                inConditional = false;
                Variable::ConditionalVariableMap.clear();
            }
        }
    }
}

std::string Compile::parseString(std::string line, int lineNumber) {

    lineNumber++;

    size_t firstInstance = line.find('"');
    if (firstInstance == std::string::npos){
        std::cout << "Line (" << lineNumber << ") contains an error: StringExt could not be parsed properly!\n";
        exit(1);
    }


    size_t lastInstance = line.find_last_of('"');
    if (lastInstance == std::string::npos){
        std::cout << "Line (" << lineNumber << ") contains an error: StringExt could not be parsed properly!\n";
        exit(1);
    }

    firstInstance++;
    String parsedString(line.substr(firstInstance, (lastInstance - firstInstance)));

    for (int j = 0; j < parsedString.ToString().length(); j++){
        char c = parsedString.ToString()[j];
        if (c == '\\'){ // Literal
            switch (parsedString.ToString()[j + 1]){
                case 'n':
                    parsedString.ptr_ReplaceMulti("\\n", "\n");

                    break;
                default:
                    break;
            }
        }

    }
    return parsedString.ToString();
}

