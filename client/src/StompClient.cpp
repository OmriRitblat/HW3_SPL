#include "../include/StompClient.h"

StompClient::StompClient()
{
    connectionHandler = nullptr;
    inputHandler = new keyboardInput(eventQueue);
    logedIn = false;
}
int main(int argc, char *argv[])
{
    StompClient *client = new StompClient();
    std::thread inputThread(&keyboardInput::run, client->inputHandler); // run input from user thread
    while (1)
    {
        Frame frame = client->eventQueue.dequeue();
        if (frame.getType() == CommandType::CONNECT)
        {
            if (client->connectionHandler != nullptr && client->connectionHandler->getLogedIn())
            {
                client->c.display("user allready loged in");
                break;
            }
            size_t pos = frame.getValue("host").find(':');
            std::string host = frame.getValue("host").substr(0, pos);
            short port = std::stoi(frame.getValue("host").substr(pos + 1));
            if (client->connectionHandler == nullptr){
                client->channelNumber=new std::unordered_map<std::string, std::string>();
                client->connectionHandler = new ConnectionHandler(host, port, client->sendMessages,client->channelNumber);
            }
            if (!(*client->connectionHandler).connect())
            {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                return 1;
            }
            frame.setValueAt("host", "stomp.cs.bgu.ac.il");
        }
        if (frame.getType() == CommandType::SUMMARY)
        {
            if (client->connectionHandler->hasDataToRead())
                client->getDataFromServer();
            DataHandler data(client->server_data);
            std::string user = frame.getValue("user");
            std::string channel = frame.getValue("channel_name");
            std::string file = frame.getValue("file");
            client->c.printToFile(data.getSummary(user, channel), file);
        }
        else
        {
            if (frame.getType() == CommandType::SUBSCRIBE)
            {
                (*client->channelNumber)[frame.getValue("destination")] = std::to_string(client->channelSubCount);
                frame.setValueAt("id", std::to_string(client->channelSubCount));
                client->channelSubCount++;
            }
            if (frame.getType() == CommandType::UNSUBSCRIBE)
            {
                auto it = (*client->channelNumber).find(frame.getValue("id"));
                frame.setValueAt("id", ""+(*client->channelNumber)[frame.getValue("id")]);
                if (it != (*client->channelNumber).end())
                {
                    (*client->channelNumber).erase(it);
                }
            }
            client->receipt++;
            frame.addReceipt("receipt", client->receipt);
            client->sendMessages.put(client->receipt, frame);
            std::cout << frame.toString() << std::endl;
            if (!(*client->connectionHandler).sendLine(frame.toString()))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
            client->getDataFromServer();
            if ((*client->connectionHandler).shouldTerminate())
            {
                std::cout << "Exiting...\n"
                          << std::endl;
                delete (client->connectionHandler);
                client->connectionHandler = nullptr;
            }
        }
    }
    delete (client);
    return 0;
}
void StompClient::getDataFromServer()
{
    std::string answer;
    // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
    // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
    do
    {
        (*connectionHandler).getLine(answer);
        // std::cout << answer << std::endl;
        // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
        // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        if(answer.length()>0){
        Frame answerFrame(answer);
        Frame res = (*connectionHandler).process(answer);
        if (answerFrame.getType() == CommandType::MESSAGE)
            server_data[res.getValue("destination")].push_back(res);
        }
        answer = "";
    } while (connectionHandler->hasDataToRead());
}
StompClient::~StompClient()
{
    delete(channelNumber);
    delete (inputHandler);
    delete (connectionHandler);
}
