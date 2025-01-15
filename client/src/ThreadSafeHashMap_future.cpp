#pragma once
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>
#include "../include/ThreadSafeHashMap_future.h";

    void ThreadSafeHashMap_future::put(const int& key, const bool& value){
    std::lock_guard<std::mutex> lock(mapMutex);
    map[key] = value;
}


    // Check if a key exists
    bool ThreadSafeHashMap_future::containsKey(const int& key) const {
        std::lock_guard<std::mutex> lock(mapMutex);
        return map.find(key) != map.end();
    }

    // Remove a key-value pair
    void ThreadSafeHashMap_future::remove(const int& key) {
        std::lock_guard<std::mutex> lock(mapMutex);
        map.erase(key);
    }

    // Get the size of the map
    size_t ThreadSafeHashMap_future::size() const {
        std::lock_guard<std::mutex> lock(mapMutex);
        return map.size();
    }

    // Clear the map
    void ThreadSafeHashMap_future::clear() {
        std::lock_guard<std::mutex> lock(mapMutex);
        map.clear();
    }