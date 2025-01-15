#pragma once
#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>

template <typename Key, typename Value>
class ThreadSafeHashMap {
private:
    std::unordered_map<Key, Value> map;
    mutable std::mutex mapMutex;

public:
    // Add or update a key-value pair
    void put(const Key& key, const Value& value);

    // Retrieve a value by key
    std::optional<Value> get(const Key& key) const;

    // Check if a key exists
    bool containsKey(const Key& key) const;

    // Remove a key-value pair
    void remove(const Key& key);

    // Get the size of the map
    size_t size() const;

    // Clear the map
    void clear();

    // Apply a function to each key-value pair
    void forEach(const std::function<void(const Key&, const Value&)>& func) const;
};