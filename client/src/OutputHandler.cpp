#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "../include/OutputHandler.h"

void OutputHandler::display(const std::string &message) const
{
    std::cout << message << std::endl;
}

void OutputHandler::printToFile(const std::string &data, const std::string &fileName) const
{
    ofstream file(fileName);
    file << data;
    file.close();
}