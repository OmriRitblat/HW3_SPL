#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include "../include/event.h"
#include "../include/keyboardInput.h"
#include "../include/ThreadSafeQueue.h"
#include <unordered_map>

keyboardInput::keyboardInput(ThreadSafeQueue &q) : eventsFromUser(q), userName("") {}

void keyboardInput::run(ThreadSafeQueue &t)
{
    while (1)
    {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string input(buf);
        createEvent(input);
    }
}

void keyboardInput::createEvent(const std::string &e)
{
    std::istringstream input(e);
    std::string command, arg1, arg2, arg3, endMsg;
    input >> command;

    std::ostringstream frame;
    if (command == "login")
    {
        input >> arg1 >> arg2 >> arg3 >> endMsg; // host:port, username, password
        if (arg1 == "" || arg2 == "" || arg3 == "")
            frame << "ERROR\n"
                  << "one of the argument is missing, you should insert port, username, password";
        else if (endMsg != "")
            frame << "ERROR\n"
                  << "there is unnessery data, you should insert port, username, password";
        else
        {
            frame << "CONNECT\n"
                  << "accept-version:1.2\n"
                  << "host:" << arg1 << "\n"
                  << "login:" << arg2 << "\n"
                  << "passcode:" << arg3 << "\n^@";
            userName = arg2;
        }
        sendFrame(frame.str());
    }
    else if (command == "join")
    {
        input >> arg1 >> endMsg; // channel_name
        if (arg1 == "")
            frame << "ERROR\n"
                  << "channel name is missing, you should insert channel name";
        else if (endMsg != "")
            frame << "ERROR\n"
                  << "there is unnessery data, you should insert channel name";
        else
        {
            frame << "SUBSCRIBE\n"
                  << "destination:/" << arg1 << "\n"
                  << "id:unique_id\n"
                  << "receipt:receipt_id\n^@";
        }
        sendFrame(frame.str());
    }
    else if (command == "exit")
    {
        input >> arg1 >> endMsg; // channel_name
        if (arg1 == "")
            frame << "ERROR\n"
                  << "channel name is missing, you should insert channel name";
        else if (endMsg != "")
            frame << "ERROR\n"
                  << "there is unnessery data, you should insert channel name";
        else
        {
            frame << "UNSUBSCRIBE\n"
                  << "id:unique_id\n"
                  << "receipt:receipt_id\n^@";
        }
        sendFrame(frame.str());
    }
    else if (command == "report")
    {
        input >> arg1 >> endMsg; // file
        if (arg1 == "")
        {
            frame << "ERROR\n"
                  << "file name is missing, you should insert file name";
            sendFrame(frame.str());
        }
        else if (endMsg != "")
        {
            frame << "ERROR\n"
                  << "there is unnessery data, you should insert file name";
            sendFrame(frame.str());
        }
        else
        {
            names_and_events allEvents = parseEventsFile(arg1);
            for (size_t i = 0; i < allEvents.events.size(); ++i)
            {
                frame.clear();
                const Event &event = allEvents.events[i];
                frame << "SEND" << "\n"
                      << "destination:" << allEvents.channel_name << "\n\n"
                      << "user: " << userName << "\n"
                      << "city: " << event.get_city() << "\n"
                      << "event name: " << event.get_name() << "\n"
                      << "date time: " << event.get_date_time() << "\n"
                      << "genetal information: " << "\n"
                      << "\t" << "active: " << event.get_general_information().at("active") << "\n"
                      << "\t" << "forces_arrival_at_scene: " << event.get_general_information().at("forces_arrival_at_scene") << "\n"
                      << "Description: " << "\n"
                      << event.get_description() << "\n";
                sendFrame(frame.str());
            }
        }
    }
    else if (command == "logout")
    {
        input >> endMsg; // channel_name
        if (endMsg != "")
            frame << "ERROR\n"
                  << "there is unnessery data, you should insert only logout command";
        else
        {
            frame << "DISCONNECT\n"
                  << "receipt:receipt_id\n^@";
            sendFrame(frame.str());
        }
    }
    else
    {
        frame << "ERROR\n"
              << "invalid command, you can use the commands: login, join, exit, report and logout";
    }
}
void keyboardInput::sendFrame(const std::string &frame)
{
    eventsFromUser.enqueue(frame);
}
