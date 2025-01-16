#pragma once
#include "../include/Frame.h"
#include <list>
#include <unordered_map>

class DataHandler
{
private:
    std::unordered_map<std::string, std::list<Frame>> &serverMessages;

public:
    void sendAllData(std::unordered_map<std::string, std::list<Frame>> &serverResponses,const std::string& user,const std::string& channel_name,const std::string& fileName)const;
};