#pragma once

#include <string>
#include <iostream>
#include <event.h>
#include "../include/ThreadSafeQueue.h"


class keyboardInput
{
private:

public:
    keyboardInput();
    virtual ~keyboardInput();
    void run(ThreadSafeQueue &t);
    std::string &CreateEvent(const std::string& e);
};