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
    void run();
    void sendFrame(Frame &frame);
    void createEvent(const std::string &e);
};