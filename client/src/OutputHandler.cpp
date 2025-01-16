#include <string>
#include <iostream>
#include "../include/OutputHandler.h"

void OutputHandler::display(const std::string &message) const
{
    std::cout << message << std::endl;
}