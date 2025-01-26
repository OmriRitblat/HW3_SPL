#ifndef SYNCHRONIZED_HASH_MAP_H
#define SYNCHRONIZED_HASH_MAP_H

#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>
#include <optional> // For std::optional

class SynchronizedHashMap
{
private:
    std::unordered_map<std::string, std::string> map; // Actual hashmap
    mutable std::mutex mapMutex;                      // Mutex for synchronization

public:
    // Constructor
    SynchronizedHashMap() = default;

    // Destructor
    ~SynchronizedHashMap() = default;

    // Disable copy constructor and assignment operator
    SynchronizedHashMap(const SynchronizedHashMap &) = delete;
    SynchronizedHashMap &operator=(const SynchronizedHashMap &) = delete;

    // Add or update a key-value pair
    void put(const std::string &key, const std::string &value);

    // Retrieve the value for a given key (returns std::optional)
    std::string get(const std::string &key) const;

    // Remove a key-value pair
    bool remove(const std::string &key);

    // Check if the map contains a key
    bool containsKey(const std::string &key) const;

    // Get the size of the map
    size_t size() const;

    // Clear all elements from the map
    void clear();

    // Print all elements in the map (for debugging)
    void printAll() const;

    std::vector<std::pair<std::string, std::string>> getAllPairs() const;
};

#endif // SYNCHRONIZED_HASH_MAP_H
