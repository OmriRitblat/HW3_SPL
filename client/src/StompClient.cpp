#include "../include/StompClient.h"

StompClient::StompClient()
{
    logedIn = false;
}
int main(int argc, char *argv[])
{
    OutputHandler * c=new OutputHandler();
    ThreadSafeHashMap_future *sendMessages=new ThreadSafeHashMap_future();
    std::unordered_map<std::string, std::list<Frame>> *server_data=new std::unordered_map<std::string, std::list<Frame>>();
    keyboardInput k(server_data,sendMessages,c);
    std::thread inputThread(&keyboardInput::run, &k); // run input from user thread
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ConnectionHandler * connectionHandler=k.getConnectionHendler();
            while(1){
                std::string answer;
                if((*connectionHandler).hasDataToRead()){
                (*connectionHandler).getLine(answer);
                int len=answer.length();
                answer.resize(len-1);
                if(answer.length()>0){
                Frame answerFrame(answer);
                Frame res = (*connectionHandler).process(answer);
                if (answerFrame.getType() == CommandType::MESSAGE)
                    (*server_data)[res.getValue("destination").substr(1)].push_back(res);
                }
                }
                if ((*connectionHandler).shouldTerminate())
            {
                std::cout << "Exiting...\n"
                          << std::endl;
                delete (connectionHandler);
                connectionHandler = nullptr;
            }
            }
    return 0;
}