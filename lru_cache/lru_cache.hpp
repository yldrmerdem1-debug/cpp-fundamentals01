#pragma once
#include <cstddef>
#include <functional>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

// Fixed-capacity cache that evicts the least recently used entry.
// get() and put() are O(1): a list keeps the recency order and a hash map points into it,
// so moving an entry to the front is a splice instead of a search.
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class LruCache {
public:
    explicit LruCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0) throw std::invalid_argument("LruCache capacity must be positive");
    }

    std::optional<Value> get(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) {
            ++misses_;
            return std::nullopt;
        }
        entries_.splice(entries_.begin(), entries_, it->second);
        ++hits_;
        return it->second->second;
    }

    void put(const Key& key, Value value) {
        auto it = index_.find(key);
        if (it != index_.end()) {
            it->second->second = std::move(value);
            entries_.splice(entries_.begin(), entries_, it->second);
            return;
        }
        if (entries_.size() == capacity_) {
            index_.erase(entries_.back().first);
            entries_.pop_back();
        }
        entries_.emplace_front(key, std::move(value));
        index_[key] = entries_.begin();
    }

    bool erase(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return false;
        entries_.erase(it->second);
        index_.erase(it);
        return true;
    }

    // Does not count as a use: the entry keeps its place in the eviction order.
    bool contains(const Key& key) const { return index_.count(key) != 0; }

    std::size_t size() const { return entries_.size(); }
    std::size_t capacity() const { return capacity_; }
    std::size_t hits() const { return hits_; }
    std::size_t misses() const { return misses_; }

private:
    using Entry = std::pair<Key, Value>;

    std::size_t capacity_;
    std::list<Entry> entries_;  // front = most recently used
    std::unordered_map<Key, typename std::list<Entry>::iterator, Hash> index_;
    std::size_t hits_ = 0;
    std::size_t misses_ = 0;
};
