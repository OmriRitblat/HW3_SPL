#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include "../include/event.h"
#include "../include/keyboardInput.h"
#include "../include/ThreadSafeQueue.h"
#include "../include/OutputHandler.h"
#include <unordered_map>
#include "../include/Frame.h"

keyboardInput::keyboardInput(ThreadSafeQueue &q) : eventsFromUser(q), userName("") {}

void keyboardInput::run()
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
    OutputHandler c;
    std::istringstream input(e);
    std::string command, arg1, arg2, arg3, endMsg;
    input >> command;

    std::ostringstream frame;
    if (command == "login")
    {
        input >> arg1 >> arg2 >> arg3 >> endMsg; // host:port, username, password
        if (arg1 == "" || arg2 == "" || arg3 == "")
            c.display("one of the argument is missing, you should insert port, username, password");
        else if (endMsg != "")
            c.display("there is unnessery data, you should insert port, username, password");
        else
        {
            frame << "CONNECT\n"
                  << "accept-version:1.2\n"
                  << "host:" << arg1 << "\n"
                  << "login:" << arg2 << "\n"
                  << "passcode:" << arg3 << "\n";
            userName = arg2;
            Frame f(frame.str());
            sendFrame(f);
        }
    }
    else if (command == "join")
    {
        input >> arg1 >> endMsg; // channel_name
        if (arg1 == "")
            c.display("channel name is missing, you should insert channel name");
        else if (endMsg != "")
            c.display("there is unnessery data, you should insert channel name");
        else
        {
            frame << "SUBSCRIBE\n"
                  << "destination:/topic/" << arg1 << "\n"
                  << "id:-";
            Frame f(frame.str());
            sendFrame(f);
        }
    }
    else if (command == "summary")
    {
        input >> arg1 >>arg2>>arg3>> endMsg; // channel_name, user, file
        if (arg1 == ""||arg2==""||arg3=="")
            c.display("channel name is missing, you should insert channel name, user and output file");
        else if (endMsg != "")
            c.display("there is unnessery data, you should insert channel name, user and output file");
        else
        {
             frame << "SUMMARY\n"
                  << "channel_name:"<<arg1<<"\n"
                  << "user:"<<arg2<<"\n"
                  << "file:"<<arg3<<"\n";
            Frame f(frame.str());
            sendFrame(f);
        }
    }
    else if (command == "exit")
    {
        input >> arg1 >> endMsg; // channel_name
        if (arg1 == "")
            c.display("channel name is missing, you should insert channel name");
        else if (endMsg != "")
            c.display("there is unnessery data, you should insert channel name");
        else
        {
            frame << "UNSUBSCRIBE\n"
                  <<"id:"<<arg1<<"\n";
            Frame f(frame.str());
            sendFrame(f);
        }
    }
    else if (command == "report")
    {
        input >> arg1 >> endMsg; // file
        if (arg1 == "")
        {
            c.display("file name is missing, you should insert file name");
        }
        else if (endMsg != "")
        {
            c.display("there is unnessery data, you should insert file name");
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
                      << "user:" << userName << "\n"
                      << "city:" << event.get_city() << "\n"
                      << "event name:" << event.get_name() << "\n"
                      << "date time:" << event.get_date_time() << "\n"
                      << "genetal information:" << "\n"
                      << "\t" << "active:" << event.get_general_information().at("active") << "\n"
                      << "\t" << "forces_arrival_at_scene:" << event.get_general_information().at("forces_arrival_at_scene") << "\n"
                      << "Description:" << "\n"
                      << event.get_description() << "\n";
                Frame f(frame.str());
                sendFrame(f);
            }
        }
    }
    else if (command == "logout")
    {
        input >> endMsg;
        if (endMsg != "")
            c.display("there is unnessery data, you should insert only logout command");
        else
        {
            frame << "DISCONNECT\n";
            Frame f(frame.str());
            sendFrame(f);
        }
    }
    else
    {
        c.display("invalid command, you can use the commands: login, join, exit, report and logout");
    }
}
void keyboardInput::sendFrame(Frame &frame)
{
    try {
        eventsFromUser.enqueue(frame);        // Enqueue the frame string
    } catch (const std::exception &e) {
        std::cerr << "Error in sendFrame: " << e.what() << std::endl;
    }
}
