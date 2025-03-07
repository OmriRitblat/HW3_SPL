#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

enum CommandType
{
    ERROR,
    CONNECTED,
    MESSAGE,
    RECEIPT,
    CONNECT,
    SUBSCRIBE,
    UNSUBSCRIBE,
    SEND,
    DISCONNECT,
    SUMMARY,
    Null
};
class Frame
{
private:
    CommandType type;
    std::unordered_map<std::string, std::string> data;
    std::string body;
    int receipt;
    std::string toStringVal;

public:
    Frame() = default;
    Frame(const std::string &inputString);
    void parseStringToHashMap(const std::string &inputString);
    const std::string &getValue(const std::string &key) const;
    const CommandType &getType() const;
    const void addReceipt(std::string key, int value);
    std::string& toString();
    std::string typeToString(CommandType s);
    int getRecipt() const;
    void setValueAt(const std::string&,const std::string&);
    std::string getBody();
    std::string getValueFromBody(std::string s) const;
};