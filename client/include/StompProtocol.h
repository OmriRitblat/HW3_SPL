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
    ThreadSafeHashMap_future &recieptMap;
    bool terminate;
    bool logedIn;

public:
    StompProtocol(ThreadSafeHashMap_future &);
    Frame process(std::string msg);
    void handelRecipt(const Frame &);
    bool shouldTerminate();
    void setTerminate();
    bool getLogedIn();
    bool setLogedIn(bool logedin);
};
