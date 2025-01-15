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
    DISCONNECT

};
class Frame
{
private:
    CommandType type;
    std::unordered_map<std::string, std::string> data;

public:
    Frame(const std::string &inputString);

    void parseStringToHashMap(const std::string &inputString);

    const std::string &getValue(const std::string &key) const;
<<<<<<< HEAD
    const CommandType &Frame::getType() const;
=======
    const ServerResponseType &getType() const;
    const void addFiled(std::string key, std::string value);
    std::string& toString();
    std::string typeToString(ServerResponseType s);
>>>>>>> refs/remotes/origin/main
};