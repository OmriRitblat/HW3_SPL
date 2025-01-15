#pragma once

#include <iostream>
#include <string>
#include "../include/Frame.h"
#include <list>
#include <unordered_map>
#include "../include/ThreadSafeHashMap_future.h"


// TODO: implement the STOMP protocol
class StompProtocol
{
private:
std::unordered_map<std::string,std::list<Frame>> serverResponses;
bool terminate;
public:
    StompProtocol(std::unordered_map<std::string,std::list<Frame>>&,ThreadSafeHashMap_future f);
    std::string process(std::string msg); 
    bool shouldTerminate();
};
