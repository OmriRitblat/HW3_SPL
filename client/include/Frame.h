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
    int receipt;

public:
    Frame(const std::string &inputString);
    
    void parseStringToHashMap(const std::string &inputString);

    const std::string &getValue(const std::string &key) const;
    const CommandType &Frame::getType() const;
<<<<<<< HEAD
    const void addFiled(std::string key, std::string value);
    std::string& toString();
    std::string typeToString(CommandType s);
=======
    const void addReceipt(std::string key, int value);
    std::string& toString();
    std::string typeToString(CommandType s);
    int getRecipt() const;
>>>>>>> refs/remotes/origin/main
};