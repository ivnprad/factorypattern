#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <memory>
#include <functional>

template<typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container = std::vector>
struct Adaptor{
    Container<T> my_data;
    void push_back(T const& t){ my_data.push_back(t); }
    T& at(size_t element){return my_data.at(element);}
};


template<typename KEY,typename T, template<typename U, typename AllocatorType = std::allocator<U>> typename Container = std::vector>
struct MyMap{
public:
    // lets first work with KEY = int 
    //Container<T> my_data;
    Container<std::pair<KEY,T>> my_data;
    T& operator[](const KEY& key) {
        auto it = find(key);
        if (it == my_data.end()) {
            my_data.emplace_back(key, T{});
            it = find(key); // in order to return a valid pointer
        }
        return it->second;
    }
    const T& operator[](const KEY& key) const {
        auto it = find(key);
        if (it == my_data.end()) {
            my_data.emplace_back(key, T{});
            it = find(key); // in order to return a valid pointer
        }
        return it->second;
    }
    //void push_back(T const& t){ my_data.push_back(t); }
    //T& at(size_t element){return my_data.at(element);}
    void insert(const std::pair<KEY, T>& keyval){
        my_data.emplace_back(keyval);
    }
    
    void erase(const KEY& key){
        auto it = find(key);
        if (it != my_data.end()) {
            my_data.erase(it);
        }        
    }

private:
    const std::function<size_t(const KEY&)> hash_fn = [](const KEY& key){return std::hash<KEY>()(key);};

    typename Container<std::pair<KEY,T>>::iterator find(const KEY& key) {
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
    // struct hash_fn {
    //     size_t operator()(const KEY& key) const {
    //         return std::hash<KEY>()(key);
    //     }
    // };
};


/*

#include <vector>
#include <iostream>

// A simple hash function that computes the index of a key-value pair
// in the vector based on its key
int hash(int key) {
    return key % 10; // The vector size is set to 10 in this example
}

int main() {
    std::vector<std::pair<int, int>> my_map(10); // Initialize the vector with a size of 10

    // Add key-value pairs to the map
    my_map[hash(1)] = std::make_pair(1, 10);
    my_map[hash(2)] = std::make_pair(2, 20);
    my_map[hash(3)] = std::make_pair(3, 30);

    // Print the value associated with a given key
    int key = 2;
    std::cout << "Value associated with key " << key << ": " << my_map[hash(key)].second << std::endl;

    return 0;
}
*/

#endif