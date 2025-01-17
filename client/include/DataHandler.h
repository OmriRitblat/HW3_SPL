#pragma once
#include "../include/Frame.h"
#include <list>
#include <unordered_map>

class DataHandler
{
private:
    std::unordered_map<std::string, std::list<Frame>> &serverMessages;

public:
    DataHandler(std::unordered_map<std::string, std::list<Frame>> &);
    std::string getSummary(const std::string &, const std::string &) const;
    std::string formatDateTime(std::string) const;
    std::string formatSummary(std::string) const;
};