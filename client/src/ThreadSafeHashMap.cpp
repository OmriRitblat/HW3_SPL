#pragma once
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>
#include "../include/ThreadSafeHashMap.h";

    template <typename Key, typename Value>
    void ThreadSafeHashMap<Key, Value>::put(const Key& key, const Value& value) {
    std::lock_guard<std::mutex> lock(mapMutex);
    map[key] = value;
}

    // Retrieve a value by key
    Value ThreadSafeHashMap::get(const Key& key) const {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second;
        }
        return std::nullopt; // Return empty optional if the key is not found
    }

    // Check if a key exists
    bool ThreadSafeHashMap::containsKey(const Key& key) const {
        std::lock_guard<std::mutex> lock(mapMutex);
        return map.find(key) != map.end();
    }

    // Remove a key-value pair
    void ThreadSafeHashMap::remove(const Key& key) {
        std::lock_guard<std::mutex> lock(mapMutex);
        map.erase(key);
    }

    // Get the size of the map
    size_t ThreadSafeHashMap::size() const {
        std::lock_guard<std::mutex> lock(mapMutex);
        return map.size();
    }

    // Clear the map
    void ThreadSafeHashMap::clear() {
        std::lock_guard<std::mutex> lock(mapMutex);
        map.clear();
    }

    // Apply a function to each key-value pair
    void ThreadSafeHashMap::forEach(const std::function<void(const Key&, const Value&)>& func) const {
        std::lock_guard<std::mutex> lock(mapMutex);
        for (const auto& [key, value] : map) {
            func(key, value);
        }
    }
};