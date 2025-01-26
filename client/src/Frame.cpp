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
    size_t delimiterPos = inputString.find("\n\n");
    std::string values = inputString.substr(0, delimiterPos);
    if (delimiterPos != std::string::npos && delimiterPos + 2 < inputString.size()) {
    body = inputString.substr(delimiterPos + 2);
    } else {
    body = "";
    }
    std::istringstream ValuesStream(values);
    std::string type;
    std::string line;
    // Extract the first line
    if (std::getline(ValuesStream, type))
    {
        if (type == "ERROR")
            this->type = ERROR;
        else if (type == "CONNECTED")
            this->type = CONNECTED;
        else if (type == "MESSAGE")
            this->type = MESSAGE;
        else if (type == "CONNECT")
            this->type = CONNECT;
        else if (type == "SUMMARY")
            this->type = SUMMARY;
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
    else{
        this->type = Null;
    }
    
    // Loop through the remaining lines
    while (std::getline(ValuesStream, line))
    {
        if (!line.empty())
        {
            size_t pos = line.find(':');
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            if (key=="receipt")
            {
                receipt=std::stoi(value);
            }else{
                data[key] = value;
            }
        }
        else
        {
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
std::string& Frame::toString() {
    // Start with the type
    toStringVal = typeToString(type) + "\n";
    for (const auto& [key, value] : data) {
            toStringVal += key + ":" + value + "\n";
    }
        toStringVal+="\n" + body;
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
    std::string Frame::getBody(){
        return body;
    }
    std::string Frame::getValueFromBody(const std::string s) const{
    std::string key = s+":";
    if(s=="Description")
        key+="\n";
    size_t keyPos = body.find(key);
    if (keyPos != std::string::npos) {
        size_t valueStart = keyPos + key.length();
        size_t valueEnd = body.find('\n', valueStart);
        if (valueEnd == std::string::npos) {
            valueEnd = body.length(); 
        }
        return body.substr(valueStart, valueEnd - valueStart);
    }
    return "";
    }