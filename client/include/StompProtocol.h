#pragma once

#include <iostream>
#include <string>
#include "../include/Frame.h"
#include <list>
#include "../include/SynchronizedHashMap.h"
#include <unordered_map>
#include "../include/ThreadSafeHashMap_future.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    ThreadSafeHashMap_future &recieptMap;
    bool terminate;
    bool logedIn;
    SynchronizedHashMap *channelNumber;

public:
    StompProtocol(ThreadSafeHashMap_future &f, SynchronizedHashMap *channelNumber);
    Frame process(std::string msg);
    void handelRecipt(const Frame &);
    bool shouldTerminate();
    void setTerminate();
    bool getLogedIn();
    bool setLogedIn(bool logedin);
    std::string findKeyByValue(SynchronizedHashMap *myMap, std::string &valueToFind);
};
