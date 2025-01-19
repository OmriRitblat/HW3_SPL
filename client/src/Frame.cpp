#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "../include/Frame.h"

Frame::Frame(const std::string &inputString)
{
    parseStringToHashMap(inputString);
}
void Frame::setValueAt(const std::string& field,const std::string& value){
    data[field]=value;
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
            size_t pos = line.find(':');
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            if (key=="receipt-id")
            {
                receipt=std::stoi(value);
            }else{
                data[key] = value;
            }
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
    const void Frame::addReceipt(std::string key, int value) {
    std::string val = std::to_string(value); 
    data.insert({key, val}); 
    receipt = value; 
}
    std::string& Frame::toString(){
        toStringVal=typeToString(type)+"\n";
        for (const auto& [key, value] : data) {
        toStringVal+=""+key+":"+""+value+"\n";
    }
    return toStringVal;
    }
    std::string Frame::typeToString(CommandType s) {
    switch (s) {
        case CommandType::ERROR:   return "ERROR";
        case CommandType::CONNECTED:   return "CONNECTED";
        case CommandType::MESSAGE:   return "MESSAGE";
        case CommandType::RECEIPT:   return "RECEIPT";
        case CommandType::CONNECT:   return "CONNECT";
        case CommandType::SUBSCRIBE:   return "SUBSCRIBE";
        case CommandType::UNSUBSCRIBE:   return "UNSUBSCRIBE";
        case CommandType::SEND:   return "SEND";
        case CommandType::DISCONNECT:   return "DISCONNECT";
        default:           return "Unknown";
    }
}
int Frame::getRecipt() const{
        return receipt;
    }
