#pragma once

#include <string>
#include <iostream>
#include <event.h>
#include "../include/ThreadSafeQueue.h"


class keyboardInput
{
private:
    ThreadSafeQueue &eventsFromUser;
    std::string userName;

public:
    keyboardInput(ThreadSafeQueue &q);
    virtual ~keyboardInput();
    void run(ThreadSafeQueue &t);
    void sendFrame(const std::string& frame);
    void createEvent(const std::string& e);
};