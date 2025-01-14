#pragma once

#include "../include/ConnectionHandler.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
bool terminate;
public:
    StompProtocol();
    std::string process(std::string msg); 
    bool shouldTerminate();
};
