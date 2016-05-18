#include <iostream>
#include <mutex>
#include <vector>
#include <forward_list>
#include <atomic>
#include <algorithm>

template<class T, class Hash = std::hash<T>>
class striped_hash_set {

    typedef std::forward_list<T>& listref;
    typedef const std::forward_list<T>& clistref;

public:

    striped_hash_set (size_t mutexNum_ = 1,
                      size_t growthFactor_ = 2,
                      double loadFactor_ = 100.0f)
    : growthFactor(growthFactor_),
    loadFactor(loadFactor_),
    locks(mutexNum_),
    size(0),
    table(67 * mutexNum_)
    {}

    void add(const T& elem);
    void remove(const T& elem);
    bool contains(const T& elem) const;

private:

    inline size_t get_bucket_index(size_t hash_value) const;
    inline size_t get_stripe_index(size_t hash_value) const;

    const size_t growthFactor;
    const double loadFactor;

    mutable std::vector<std::mutex> locks;

    std::atomic<size_t> size;
    std::vector<std::forward_list<T>> table;
};
