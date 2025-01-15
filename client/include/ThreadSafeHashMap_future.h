#pragma once
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>

class ThreadSafeHashMap_future {
private:
    std::unordered_map<int, bool> map;
    mutable std::mutex mapMutex;

public:
    // Add or update a key-value pair
    void put(const int& key, const bool& value);

    // Retrieve a value by key
    bool get(const int& key) const;

    // Check if a key exists
    bool containsKey(const int& key) const;

    // Remove a key-value pair
    void remove(const int& key);

    // Get the size of the map
    size_t size() const;

    // Clear the map
    void clear();

};