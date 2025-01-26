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
                if((*connectionHandler).getLine(answer)){
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
<<<<<<< HEAD
void StompClient::getDataFromServer()
{

    std::string answer;
    // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
    // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
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
=======
>>>>>>> refs/remotes/origin/main
