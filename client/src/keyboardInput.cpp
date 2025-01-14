 #pragma once

#include <string>
#include <iostream>
#include "../include/event.h"
#include "../include/keyboardInput.h"
#include "../include/ThreadSafeQueue.h"


void keyboardInput::run(ThreadSafeQueue &t){
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string input(buf);
        t.enqueue(keyboardInput::CreateEvent(input));
    }
}

std::string& keyboardInput::CreateEvent(const std::string& e) {
    //to do
}
