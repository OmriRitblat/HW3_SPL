#pragma once
#include <iostream>
#include <string>
// #include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <list>
#include <unordered_map>
#include "../include/CLI.h"
StompProtocol::StompProtocol(std::unordered_map<std::string, std::list<Frame>> &respoonses, ThreadSafeHashMap_future &recieptMap) : terminate(false)
{
    // Initialization or any other setup
}
std::string StompProtocol::process(std::string msg)
{
    Frame serverMessage(msg);
    CLI c;
    switch (serverMessage.getType())
    {
    case ERROR:
        c.display("Error :\n");
        c.display(serverMessage.getValue("body"));
        break;
    case MESSAGE:
        string key = serverMessage.getValue("subscribtion");
        data[key].push_back(serverMessage);
        break;
    case CONNECTED:
        c.display("Login succcessful");
        break;
    case RECEIPT:
        handelRecipt(serverMessage);
        break;
    default:
        break;
    }
}
bool StompProtocol::shouldTerminate()
{
}
void StompProtocol::handelRecipt(const Frame &serverMessage)
{
}
