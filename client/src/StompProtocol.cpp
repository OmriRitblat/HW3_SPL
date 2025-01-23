#pragma once
#include <iostream>
#include <string>
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <list>
#include <unordered_map>
#include "../include/OutputHandler.h"
StompProtocol::StompProtocol(ThreadSafeHashMap_future &f) : recieptMap(f), terminate(false), logedIn(false) {}
Frame StompProtocol::process(std::string msg)
{
    Frame serverMessage(msg);
    OutputHandler c;
    switch (serverMessage.getType())
    {
    case CommandType::ERROR:
        c.display("Error :\n");
        c.display(serverMessage.getValue("message"));
        terminate = true;
        break;
    case CommandType::MESSAGE:
        return serverMessage; // for updating the hashmap of events
        break;
    case CommandType::CONNECTED:
        c.display("Login succcessful");
        logedIn = true;
        break;
    case CommandType::RECEIPT:
        handelRecipt(serverMessage);
        break;
    default:
        break;
    }
    return Frame("Null");
}
bool StompProtocol::shouldTerminate()
{
    return terminate;
}
void StompProtocol::handelRecipt(const Frame &serverMessage)
{
    Frame f = recieptMap.get(serverMessage.getRecipt());
    OutputHandler c;
    switch (f.getType())
    {
    case CommandType::SUBSCRIBE:
    {
        size_t pos = f.getValue("destination").find_last_of('/');
        std::string result = (pos != std::string::npos) ? f.getValue("destination").substr(pos + 1) : f.getValue("destination");
        c.display("Joined channel " + result);
        break;
    }
    case CommandType::UNSUBSCRIBE:
    {
        size_t pos = f.getValue("destination").find_last_of('/');
        std::string result = (pos != std::string::npos) ? f.getValue("destination").substr(pos + 1) : f.getValue("destination");
        c.display("Exited channel " + result);
        break;
    }
    case CommandType::DISCONNECT:
        terminate = true;
        break;
    default:
        break;
    }
}
void StompProtocol::setTerminate()
{
    terminate = true;
}
bool StompProtocol::getLogedIn()
{
    return logedIn;
}
