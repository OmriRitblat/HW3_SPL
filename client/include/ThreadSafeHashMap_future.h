#pragma once
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>
#include "../include/Frame.h"

class ThreadSafeHashMap_future {
private:
    std::unordered_map<int, Frame> map;
    mutable std::mutex mapMutex;

public:
    // Add or update a key-value pair
    void put(const int& key, const Frame& value);

    // Retrieve a value by key
    Frame get(const int& key) const;

    // Check if a key exists
    bool containsKey(const int& key) const;

    // Remove a key-value pair
    void remove(const int& key);

    // Get the size of the map
    size_t size() const;

    // Clear the map
    void clear();

};