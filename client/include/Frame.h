#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

enum ServerResponseType
{
    ERROR,
    CONNECTED,
    MESSAGE,
    RECEIPT
};
class Frame
{
private:
    ServerResponseType type;
    std::unordered_map<std::string, std::string> data;

public:
    Frame(const std::string &inputString);

    void parseStringToHashMap(const std::string &inputString);

    const std::string &getValue(const std::string &key) const;
    const ServerResponseType &Frame::getType() const;
};