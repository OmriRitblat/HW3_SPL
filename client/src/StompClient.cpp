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
    OutputHandler c;
    ThreadSafeHashMap_future sendMessages; // map of recip id and the frame
    std::unordered_map<std::string, std::list<Frame>> server_data; //<chanel name ,all frames send to this chanel>
    std::unordered_map<std::string, std::string> channelNumber;    //<channel name,subscibtion id>
    int channelSubCount = 0;
    ThreadSafeQueue eventQueue;
    keyboardInput inputHandler(eventQueue);
    int receipt = 0;
    ConnectionHandler* connectionHandler;
    std::thread inputThread(&keyboardInput::run, &inputHandler); // run input from user thread
    while (1)
    {
        Frame frame = eventQueue.dequeue();
        if(frame.getType()==CommandType::CONNECT){
            size_t pos = frame.getValue("host").find(':');
            std::string host = frame.getValue("host").substr(0, pos);
            short port = std::stoi(frame.getValue("host").substr(pos + 1));
            connectionHandler =new ConnectionHandler(host, port, sendMessages);
            if (!(*connectionHandler).connect())
            {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                return 1;
            }
            frame.setValueAt("host","stomp.cs.bgu.ac.il");
        }
        if (frame.getType() == CommandType::SUMMARY)
        {
            DataHandler data(server_data);
            std::string user = frame.getValue("user");
            std::string channel = frame.getValue("channel_name");
            std::string file = frame.getValue("file");
            c.printToFile(data.getSummary(user, channel), file);
        }
        else
        {
            if (frame.getType() == CommandType::SUBSCRIBE)
            {
                channelNumber[frame.getValue("destination")] = std::to_string(channelSubCount);
                frame.setValueAt("id",std::to_string(channelSubCount));
                channelSubCount++;
            }
            if (frame.getType() == CommandType::UNSUBSCRIBE)
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
            std::cout << frame.toString() << std::endl;
            if (!(*connectionHandler).sendLine(frame.toString()))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
            std::string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
           
            do{
                (*connectionHandler).getLine(answer);
                    // std::cout << answer << std::endl;
                int len = answer.length();
                // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
                // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
                Frame answerFrame(answer);
                Frame res = (*connectionHandler).process(answer);
                if (answerFrame.getType() == CommandType::MESSAGE)
                    server_data[res.getValue("destination")].push_back(res);
                answer="";
            } while (connectionHandler->hasDataToRead());
            if ((*connectionHandler).shouldTerminate())
            {
                std::cout << "Exiting...\n"
                          << std::endl;
                break;
            }
        }
    }
    return 0;
}
