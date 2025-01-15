#pragma once

#include <iostream>
#include <string>
#include "../include/Frame.h"
#include <list>
#include <unordered_map>


// TODO: implement the STOMP protocol
class StompProtocol
{
private:
std::unordered_map<std::string,std::list<Frame>> serverResponses;
bool terminate;
public:
    StompProtocol(std::unordered_map<std::string,std::list<Frame>>&);
    std::string process(std::string msg); 
    bool shouldTerminate();
};
