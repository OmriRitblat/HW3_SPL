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
        if (type == "ERROR")
            this->type = ERROR;
        else if (type == "CONNECTED")
            this->type = CONNECTED;
        else if (type == "MESSAGE")
            this->type = MESSAGE;
        else if (type == "CONNECT")
            this->type = CONNECT;
        else if (type == "SUBSCRIBE")
            this->type = SUBSCRIBE;
            else if (type == "UNSUBSCRIBE")
            this->type = UNSUBSCRIBE;
        else if (type == "SEND")
            this->type = SEND;
        else if (type == "DISCONNECT")
            this->type = DISCONNECT;
        else
            this->type = RECEIPT;
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
            data["body"] = "";
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

const CommandType &Frame::getType() const
{
    return type;
}
    const void Frame::addFiled(std::string key, std::string value){
        data.insert(key,value);
    }
    std::string& Frame::toString(){
        std::string res=typeToString(type)+"\n";
        for (const auto& [key, value] : data) {
        res+=""+key+": "+""+value+"\n";
    }
    res+="^@";
    return res;
    }
    std::string Frame::typeToString(ServerResponseType s) {
    switch (s) {
        case ServerResponseType::ERROR:   return "ERROR";
        case ServerResponseType::CONNECTED:   return "CONNECTED";
        case ServerResponseType::MESSAGE:   return "MESSAGE";
        case ServerResponseType::RECEIPT:   return "RECEIPT";
        default:           return "Unknown";
    }
}
