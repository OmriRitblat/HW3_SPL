#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/OutputHandler.h"
#include "../include/DataHandler.h"
#include "../include/keyboardInput.h"
#include "../include/ThreadSafeQueue.h"
#include "../include/ThreadSafeHashMap_future.h"
#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include <thread>
#include <iostream>
#include <list>
#include <string>
using boost::asio::ip::tcp;

class StompClient {
public:
	OutputHandler c;
    ThreadSafeHashMap_future sendMessages; // map of recip id and the frame
    std::unordered_map<std::string, std::list<Frame>> server_data; //<chanel name ,all frames send to this chanel>
    std::unordered_map<std::string, std::string> channelNumber;    //<channel name,subscibtion id>
    int channelSubCount = 0;
    ThreadSafeQueue eventQueue;
    keyboardInput* inputHandler;
    int receipt = 0;
    ConnectionHandler* connectionHandler;
    StompClient();
    ~StompClient();
	void getDataFromServer();
};
