#pragma once

#include <string>
class OutputHandler
{
private:
public:
    void display(const std::string& ) const;
    void printToFile(const std::string &, const std::string &)const;
};