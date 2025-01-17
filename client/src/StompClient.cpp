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

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl
                  << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    OutputHandler c;
    ThreadSafeHashMap_future sendMessages; // map of recip id and the frame
    ConnectionHandler connectionHandler(host, port, sendMessages);
    if (!connectionHandler.connect())
    {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    std::unordered_map<std::string, std::list<Frame>> server_data; //<user name,all frames send from>
    std::unordered_map<std::string, std::string> channelNumber;    //<channel name,subscibtion id>
    int channelSubCount = 0;
    ThreadSafeQueue eventQueue;
    keyboardInput inputHandler(std::ref(eventQueue));
    int receipt = 0;
    std::thread inputThread(&keyboardInput::run, &inputHandler); // run input from user thread
    while (1)
    {
        Frame frame = eventQueue.dequeue();
        if (frame.getType() == SUMMARY)
        {
            DataHandler data(server_data);
            std::string user = frame.getValue("user");
            std::string channel = frame.getValue("channel_name");
            std::string file = frame.getValue("file");
            c.printToFile(data.getSummary(user, channel), file);
        }
        else
        {
            if (frame.getType() == SUBSCRIBE)
            {
                channelNumber[frame.getValue("destination")] = std::to_string(channelSubCount);
                channelSubCount++;
            }
            if (frame.getType() == UNSUBSCRIBE)
            {
                frame.setValueAt("id", channelNumber[frame.getValue("id")]);
                auto it = channelNumber.find("key2");

                if (it != channelNumber.end())
                {
                    channelNumber.erase(it);
                }
            }
            receipt++;
            frame.addReceipt("receipt", receipt);
            sendMessages.put(receipt, frame);
            if (!connectionHandler.sendLine(frame.toString()))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
            std::cout << "Sent " << frame.toString() << "\n" << "to server" << std::endl;
            std::string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
            if (!connectionHandler.getLine(answer))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
            int len = answer.length();
            // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
            // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
            answer.resize(len - 1);
            Frame res = connectionHandler.process(answer);
            if (res.getType() != CommandType::Null)
                server_data[res.getValue("user")].push_back(res);
            if (connectionHandler.shouldTerminate())
            {
                std::cout << "Exiting...\n"
                          << std::endl;
                break;
            }
        }
    }
    return 0;
}
