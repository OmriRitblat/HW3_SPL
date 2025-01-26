#include <unordered_map>
#include <string>
#include "../include/SynchronizedHashMap.h"
#include <mutex>
#include <optional> // For optional return values

void SynchronizedHashMap::put(const std::string &key, const std::string &value)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    map[key] = value;
}

// Retrieve the value for a given key (returns std::optional)
std::string SynchronizedHashMap::get(const std::string &key) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = map.find(key);
    if (it != map.end())
    {
        return it->second; // Key found
    }
    return ""; // Key not found
}

// Remove a key-value pair
bool SynchronizedHashMap::remove(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return map.erase(key) > 0; // Returns true if key was removed
}

// Check if the map contains a key
bool SynchronizedHashMap::containsKey(const std::string &key) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return map.find(key) != map.end();
}

// Get the size of the map
size_t SynchronizedHashMap::size() const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return map.size();
}

// Clear all elements from the map
void SynchronizedHashMap::clear()
{
    std::lock_guard<std::mutex> lock(mapMutex);
    map.clear();
}
std::vector<std::pair<std::string, std::string>> SynchronizedHashMap::getAllPairs() const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    std::vector<std::pair<std::string, std::string>> allPairs(map.begin(), map.end());
    return allPairs;
}
