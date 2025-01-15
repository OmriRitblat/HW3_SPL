#pragma once
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/Frame.h"

constexpr unsigned int hashString(const char* str, int h = 0) {
    return !str[h] ? 5381 : (hashString(str, h + 1) * 33) ^ str[h];
}

    StompProtocol::StompProtocol() : terminate(false) {
    // Initialization or any other setup
    }
    std::string StompProtocol::process(std::string msg){
        Frame f(msg);
        switch (hashString(f.getType().c_str())) {
        case hashString("CONNECT"):
            std::cout << "Login successful" << std::endl;
            break;
        case value2:
            // Code to execute if expression == value2
            break;
        // Add more cases as needed
        default:
            // Code to execute if none of the above cases match
            break;
}
    } 
    bool StompProtocol::shouldTerminate(){

    }

