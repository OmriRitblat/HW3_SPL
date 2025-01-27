#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include "../include/event.h"
#include "../include/keyboardInput.h"
#include "../include/OutputHandler.h"
#include <unordered_map>
#include "../include/Frame.h"

keyboardInput::keyboardInput(std::unordered_map<std::string, std::list<Frame>> *server_data,
                             ThreadSafeHashMap_future *sendMessages,
                             OutputHandler *c)
    : userName(""), server_data(server_data), sendMessages(sendMessages), c(c)
{
    channelNumber = new SynchronizedHashMap();
    connectionHandler = nullptr;
}
void keyboardInput::run()
{
    std::unique_lock<std::mutex> lock(mutex_);
    int receipt = 0;
    int channelSubCount = 0;
    while (1)
    {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string input(buf);
        std::list<Frame> frames = createEvent(input);
        if (frames.front().getType() == CommandType::CONNECT)
        {
            if (connectionHandler != nullptr && connectionHandler->getLogedIn())
            {
                (*c).display("Client allready loged in");
                break;
            }
            size_t pos = frames.front().getValue("host").find(':');
            std::string host = frames.front().getValue("host").substr(0, pos);
            short port = std::stoi(frames.front().getValue("host").substr(pos + 1));
            if (connectionHandler == nullptr)
            {
                connectionHandler = new ConnectionHandler(host, port, (*sendMessages), channelNumber);
            }
            if (!(*connectionHandler).connect())
            {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                break;
            }
            lock.unlock();
            frames.front().setValueAt("host", "stomp.cs.bgu.ac.il");
        }
        if (frames.front().getType() == CommandType::SUMMARY)
        {
            DataHandler data((*server_data));
            std::string user = frames.front().getValue("user");
            std::string channel = frames.front().getValue("channel_name").substr(1);
            std::string file = frames.front().getValue("file");
            if ((*channelNumber).get(channel) != "")
                (*c).printToFile(data.getSummary(user, channel), file);
        }
        else
        {
            if (frames.front().getType() == CommandType::SUBSCRIBE)
            {
                (*channelNumber).put(frames.front().getValue("destination"),std::to_string(channelSubCount));
                frames.front().setValueAt("id", std::to_string(channelSubCount));
                channelSubCount++;
            }
            if (frames.front().getType() == CommandType::UNSUBSCRIBE)
            {
                auto it = (*channelNumber).get(frames.front().getValue("id"));
                frames.front().setValueAt("id", "" + (*channelNumber).get(frames.front().getValue("id")));
                (*channelNumber).remove(it);
            }
            for (Frame &frame : frames)
            {
                receipt++;
                frame.addReceipt("receipt", receipt);
                (*sendMessages).put(receipt, frame);
                if (!(*connectionHandler).sendLine(frame.toString()))
                {
                    std::cout << "Disconnected. Exiting...\n"
                              << std::endl;
                    break;
                }
            }
        }
    }
}

std::list<Frame> keyboardInput::createEvent(const std::string &e)
{
    std::list<Frame> l;
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
            l.push_back(f);
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
                  << "destination:" << arg1 << "\n"
                  << "id:-";
            Frame f(frame.str());
            l.push_back(f);
        }
    }
    else if (command == "summary")
    {
        input >> arg1 >> arg2 >> arg3 >> endMsg; // channel_name, user, file
        if (arg1 == "" || arg2 == "" || arg3 == "")
            c.display("channel name is missing, you should insert channel name, user and output file");
        else if (endMsg != "")
            c.display("there is unnessery data, you should insert channel name, user and output file");
        else
        {
            frame << "SUMMARY\n"
                  << "channel_name:/" << arg1 << "\n"
                  << "user:" << arg2 << "\n"
                  << "file:" << arg3 << "\n";
            Frame f(frame.str());
            l.push_back(f);
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
                  << "id:" << arg1 << "\n";
            Frame f(frame.str());
            l.push_back(f);
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
                std::ostringstream frame;
                const Event &event = allEvents.events[i];
                frame << "SEND" << "\n"
                      << "destination:/" << allEvents.channel_name << "\n\n"
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
                l.push_back(f);
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
            l.push_back(f);
        }
    }
    else
    {
        c.display("invalid command, you can use the commands: login, join, exit, report and logout");
    }
    return l;
}
ConnectionHandler *keyboardInput::getConnectionHendler()
{
    std::unique_lock<std::mutex> lock(mutex_);
    return connectionHandler;
}
