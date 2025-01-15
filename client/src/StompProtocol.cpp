#pragma once
#include <iostream>
#include <string>
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <list>
#include <unordered_map>
#include "../include/CLI.h"
StompProtocol::StompProtocol(std::unordered_map<std::string, std::list<Frame>> &respoonses, ThreadSafeHashMap_future &recieptMap) : serverResponses(respoonses), recieptMap(recieptMap), terminate(false)
{
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
        std::string key = serverMessage.getValue("subscribtion");
        serverResponses[key].push_back(serverMessage);
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
    return terminate;
}
void StompProtocol::handelRecipt(const Frame &serverMessage)
{
    Frame f=recieptMap.get(serverMessage.getRecipt());
    CLI c;
    switch (f.getType())
    {
        case CommandType::SUBSCRIBE:
            c.display("Joined channel" + f.getValue("destination"));
        case CommandType::UNSUBSCRIBE:   
            c.display("Exited channel" + f.getValue("destination"));
        case CommandType::DISCONNECT:
            terminate=true;
    }
}
