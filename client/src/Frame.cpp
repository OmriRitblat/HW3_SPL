#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "../include/Frame.h"

class Frame {
private:
    std::unordered_map<std::string, std::string> hashMap;

public:
    Frame(const std::string& inputString) {
        parseStringToHashMap(inputString);
    }

    void parseStringToHashMap(const std::string& inputString) {
    }

    const std::string& getValue(const std::string& key) const {
    }

};