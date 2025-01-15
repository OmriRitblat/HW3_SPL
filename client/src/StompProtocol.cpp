#pragma once
#include <iostream>
#include <string>
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <list>
#include <unordered_map>
#include "../include/CLI.h"
StompProtocol::StompProtocol(ThreadSafeHashMap_future& f) :recieptMap(f),terminate(false){}
Frame StompProtocol::process(std::string msg)
{
    Frame serverMessage(msg);
    CLI c;
    switch (serverMessage.getType())
    {
    case CommandType::ERROR:
        c.display("Error :\n");
        c.display(serverMessage.getValue("body"));
        break;
    case CommandType::MESSAGE:
        return serverMessage;//for updating the hashmap of events
        break;
    case CommandType::CONNECTED:
        c.display("Login succcessful");
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
    Frame f=recieptMap.get(serverMessage.getRecipt());
    CLI c;
    switch (f.getType())
    {
        case CommandType::SUBSCRIBE:
            c.display("Joined channel" + f.getValue("destination"));
            break;
        case CommandType::UNSUBSCRIBE:   
            c.display("Exited channel" + f.getValue("destination"));
            break;
        case CommandType::DISCONNECT:
            terminate=true;
            break;
        default:
            break;
    }
}
