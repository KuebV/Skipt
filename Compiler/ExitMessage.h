//
// Created by bobwt on 3/7/2023.
//

#ifndef SKIPT_EXITMESSAGE_H
#define SKIPT_EXITMESSAGE_H

#include <string>
#include <utility>
#include <iostream>

class ExitMessage{
public:
    explicit ExitMessage(std::string _class){
        classFunction = std::move(_class);
    }

    void Error(std::string method, std::string message, std::string referenceLine, int errorCode){
        std::cout << "\n[Error] | [" << classFunction << "] [" << method << "]: " << message << "\n";
        std::cout << "        |> " << referenceLine << "\n";
        exit(errorCode);
    }

    std::string classFunction;
};

#endif //SKIPT_EXITMESSAGE_H
