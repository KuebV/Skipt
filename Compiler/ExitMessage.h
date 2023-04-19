//
// Created by bobwt on 3/7/2023.
//

#ifndef SKIPT_EXITMESSAGE_H
#define SKIPT_EXITMESSAGE_H

#include <string>
#include <utility>
#include <iostream>
#include "../Property/PropertyReference.h"
#include "../Property/PropertyFile.h"

class ExitMessage{
public:
    explicit ExitMessage(std::string _class){
        classFunction = std::move(_class);
        propRef = PropertyFile::ReadPropertyFile("compiler.properties");
    }

    void Error(std::string method, std::string message, std::string referenceLine, int errorCode){
        std::cout << "\n[Error] | [" << classFunction << "] [" << method << "]: " << message << "\n";
        std::cout << "        |> " << referenceLine << "\n";

        if (StringExt::ToBoolean(propRef.getValue("keepOpen"))){
            int x = 0;
            std::cin >> x;
            exit(x);
        }
    }

    std::string classFunction;
private:
    PropertyReference propRef;
};


#endif //SKIPT_EXITMESSAGE_H
