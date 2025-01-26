#pragma once

#include <string>
#include <iostream>
#include <event.h>
#include "../include/ConnectionHandler.h"
#include "../include/DataHandler.h"
#include "../include/OutputHandler.h"
#include <thread>
#include <iostream>
#include <list>
#include <mutex>
#include <string>
class keyboardInput
{
private:
    std::string userName;
    mutable std::mutex mutex_;
    ThreadSafeHashMap_future *sendMessages;                         // map of recip id and the frame
    std::unordered_map<std::string, std::list<Frame>> *server_data; //<chanel name ,all frames send to this chanel>
    SynchronizedHashMap *channelNumber;                             //<channel name,subscibtion id>
    ConnectionHandler *connectionHandler;
    OutputHandler *c;

public:
    keyboardInput(std::unordered_map<std::string, std::list<Frame>> *server_data, ThreadSafeHashMap_future *sendMessages, OutputHandler *c);
    void run();
    std::list<Frame> createEvent(const std::string &e);
    void getDataFromServer();
    ConnectionHandler *getConnectionHendler();
};