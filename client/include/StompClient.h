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
    bool logedIn;
    OutputHandler * c;
    StompClient();
    ~StompClient();
	void getDataFromServer();
};
