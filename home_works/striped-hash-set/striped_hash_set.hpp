#include "striped_hash_set.h"

template<class T, class Hash>
void striped_hash_set<T, Hash>::add(const T& key) {

    size_t hash_value = Hash()(key);
    std::unique_lock<std::mutex> lock(mutexes[get_stripe_index(hash_value)]);
    list_reference bucket = table[get_bucket_index(hash_value)];

    if(std::find(bucket.begin(), bucket.end(), key) != bucket.end())
        return;

    bucket.push_front(key);
    size++;

    double factor = static_cast<double>(size) / table.size();
    bool is_overload = (factor >= load_factor); 

    if (is_overload) {
        size_t old_size = table.size();
        lock.unlock();

        grow_up(old_size);
    }
}

template<class T, class Hash>
void striped_hash_set<T, Hash>::grow_up(size_t old_size) {
    std::vector<std::unique_lock<std::mutex>> ulocks;

    for(auto& mutex: mutexes)
        ulocks.emplace_back(mutex);

    bool does_not_changed = (old_size == table.size());

    if(does_not_changed) {
        auto old_table = std::move(table);
        size_t size = growth_factor * old_size;

        table = std::forward_list<T>(size);

        for(const auto& buck: old_table)
            for(const auto& elm: buck)
                table[Hash()(elm) % size].push_front(elm);
    }
}

template<class T, class Hash>
void striped_hash_set<T, Hash>::remove(const T& key) {

    size_t hash_value = Hash()(key);
    std::unique_lock<std::mutex> lock(mutexes[get_stripe_index(hash_value)]);

    list_reference bucket = table[get_bucket_index(hash_value)];
    bucket.remove(key);

    size--;
}

template<class T, class Hash>
bool striped_hash_set<T, Hash>::contains(const T& key) const {
    size_t hash_value = Hash()(key);
    std::unique_lock<std::mutex> lock(mutexes[get_stripe_index(hash_value)]);

    const_list_reference bucket = table[get_bucket_index(hash_value)];

    return std::find(bucket.begin(), bucket.end(), key) != bucket.end();
}

template<class T, class Hash>
size_t striped_hash_set<T, Hash>::get_bucket_index(size_t hash_value) const {
    return hash_value % table.size();
}

template<class T, class Hash>
size_t striped_hash_set<T, Hash>::get_stripe_index(size_t hash_value) const {
    return hash_value % mutexes.size();
}
