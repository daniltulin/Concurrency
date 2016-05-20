#include <iostream>
#include <mutex>
#include <vector>
#include <forward_list>
#include <atomic>
#include <algorithm>

template<class T, class Hash = std::hash<T>>
class striped_hash_set {

    typedef std::forward_list<T>& list_reference;
    typedef const std::forward_list<T>& const_list_reference;

public:

    striped_hash_set (size_t mutexes_qty = 1,
                      size_t growth_factor = 2,
                      double load_factor = 100.0f)
    : growth_factor(growth_factor),
    load_factor(load_factor),
    mutexes(mutexes_qty),
    size(0),
    table(91 * mutexes_qty)
    {}

    void add(const T& key);
    void remove(const T& key);
    bool contains(const T& key) const;

private:

    inline size_t get_bucket_index(size_t hash_value) const;
    inline size_t get_stripe_index(size_t hash_value) const;

    const size_t growth_factor;
    const double load_factor;

    mutable std::vector<std::mutex> mutexes;

    void grow_up(size_t old_size);

    std::atomic<size_t> size;
    std::vector<std::forward_list<T>> table;
};
