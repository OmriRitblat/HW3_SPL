#pragma once
#include <iostream>
#include <string>
// #include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <list>
#include <unordered_map>

StompProtocol::StompProtocol(std::unordered_map<std::string, std::list<Frame>> &respoonses) : terminate(false)
{
    // Initialization or any other setup
}
std::string StompProtocol::process(std::string msg)
{
    Frame serverMessage(msg);
    
}
bool StompProtocol::shouldTerminate()
{
}
