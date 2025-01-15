#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/keyboardInput.h"
#include "../include/ThreadSafeQueue.h"
#include "../include/StompProtocol.h"
#include <thread>
#include <iostream>
/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
	StompProtocol protocol;
    ThreadSafeQueue eventQueue;
	keyboardInput inputHandler(std::ref(eventQueue));
    std::thread inputThread(&keyboardInput::run, &inputHandler);//run input from user thread
    while (1) {
		std::string frame=eventQueue.dequeue();
        if (!connectionHandler.sendLine(frame)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        std::cout << "Sent " << frame << " /n"<<"to server" << std::endl;
        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        
		int len=answer.length();
		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        answer.resize(len-1);
		std::string res=protocol.process(answer);
        if (protocol.shouldTerminate()) {
            std::cout << "Exiting...\n" << std::endl;
            break;
        }
    }
    return 0;
}
