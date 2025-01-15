#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "../include/Frame.h"

Frame::Frame(const std::string &inputString)
{
    parseStringToHashMap(inputString);
}

void Frame::parseStringToHashMap(const std::string &inputString)
{
    std::istringstream stream(inputString);
    std::string type;
    std::string line;

    // Extract the first line
    if (std::getline(stream, type))
    {
        this->type = type;
    }

    // Loop through the remaining lines
    while (std::getline(stream, line))
    {
        if (!line.empty())
        {
            std::istringstream lineStream(line);
            std::string key, value;
            lineStream >> key;
            std::getline(lineStream, value);

            // Trim leading space from value
            if (!value.empty() && value[0] == ' ')
            {
                value.erase(0, 1);
            }
            data[key] = value;
        }
        else
        {
            std::string remainingContent;
            data["body"]="";
            while (std::getline(stream, remainingContent))
            {
                if (!remainingContent.empty())
                {
                    data["body"] += " " + remainingContent;
                }
            }

            break;
        }
    }
}

const std::string &Frame::getValue(const std::string &key) const
{
    return data.at(key);
}

const std::string &Frame::getType() const
{
    return type;
}