#ifndef VECTORMAP_HPP
#define VECTORMAP_HPP

#include <memory>

template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container = std::vector>
class vectormap {
public:
    T& operator[](const KEY& key);
    const T& operator[](const KEY& key) const;
    void insert(const std::pair<KEY, T>& keyval);
    void erase(const KEY& key);
private:
    Container<std::pair<KEY, T>> my_data;
    typename Container<std::pair<KEY, T>>::iterator find(const KEY& key);
    const std::function<size_t(const KEY&)> hash_fn = [](const KEY& key){return std::hash<KEY>()(key);};
};

template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container>
T& vectormap<KEY, T, Container>::operator[](const KEY& key) {
    auto it = find(key);
    if (it == my_data.end()) {
        my_data.emplace_back(key, T{});
        it = find(key); // in order to return a valid pointer
    }
    return it->second;
}

template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container>
const T& vectormap<KEY, T, Container>::operator[](const KEY& key) const {
    auto it = find(key);
    if (it == my_data.end()) {
        my_data.emplace_back(key, T{});
        it = find(key); // in order to return a valid pointer
    }
    return it->second;
}


template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container>
void vectormap<KEY, T, Container>::insert(const std::pair<KEY, T>& keyval) {
    my_data.emplace_back(keyval);
}

template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container>
void vectormap<KEY, T, Container>::erase(const KEY& key) {
    auto it = find(key);
    if (it != my_data.end()) {
        my_data.erase(it);
    }
}


template<typename KEY, typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container>
typename Container<std::pair<KEY, T>>::iterator vectormap<KEY, T, Container>::find(const KEY& key) {
    size_t hash_value = hash_fn(key);
    auto it = my_data.begin();
    while (it != my_data.end()) {
        if (hash_fn(it->first) == hash_value && it->first == key) {
            break;
        }
        ++it;
    }
    return it;
}


#endif 