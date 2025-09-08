#include <list>
#include <unordered_map>
#include <iterator>
#include <iostream>
#include <iostream>


template <typename K, typename V>
class LruCache {
public:
    explicit LruCache(size_t max_size)
    : size_(max_size) {}

    void Put(const K& key, const V& value) {
        if(q_.size() == size_) {
            auto last_element = q_.back();
            m_.erase(last_element);
            q_.pop_back();
        }
        q_.push_front(key);
        m_[key] = std::make_pair(q_.begin(), value);
    }

    bool Get(const K& key, V* value) {
        auto it = m_.find(key);

        if(it == m_.end()) {
            return false;
        }

        *value = m_[key].second;
        q_.erase(m_[key].first);
        q_.push_front(key);
        m_[key].first = q_.begin();

        return true;
    }
private:
    size_t size_ = 0;
    std::list<K> q_;
    std::unordered_map<K, std::pair<typename std::list<K>::iterator, V> > m_;
};
