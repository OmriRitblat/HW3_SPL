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
    std::unordered_map<std::string, std::list<Frame>> &serverResponses;
    ThreadSafeHashMap_future &recieptMap;
    bool terminate;

public:
    StompProtocol(std::unordered_map<std::string, std::list<Frame>> &, ThreadSafeHashMap_future &);
    std::string process(std::string msg);
    void handelRecipt(const Frame &);
    bool shouldTerminate();
};
