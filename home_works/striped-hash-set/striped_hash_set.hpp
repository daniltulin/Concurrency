#include "striped_hash_set.h"

template<class T, class Hash>
void striped_hash_set<T, Hash>::add(const T& elem) {

    size_t hash_value = Hash()(elem);
    std::unique_lock<std::mutex> ul(locks[get_stripe_index(hash_value)]);
    listref bucket = table[get_bucket_index(hash_value)];

    if(std::find(bucket.begin(), bucket.end(), elem) != bucket.end())
        return;

    bucket.push_front(elem);
    size++;

    double factor = static_cast<double>(size) / table.size();
    bool is_overload = (factor >= loadFactor); 

    if (is_overload) {
        size_t old_size = table.size();
        ul.unlock();

        std::vector<std::unique_lock<std::mutex>> ulocks;

        for(auto& mutex: locks)
            ulocks.emplace_back(mutex);

        bool does_not_changed = (old_size == table.size());

        if(does_not_changed) {
            auto oldTable = table;
            size_t size = growthFactor * table.size();

            table.clear();
            table.resize(size);

            for(const auto& buck: oldTable)
                for(const auto& elm: buck)
                    table[Hash()(elm) % size].push_front(elm);
        }
    }
}

template<class T, class Hash>
void striped_hash_set<T, Hash>::remove(const T& elem) {

    size_t hash_value = Hash()(elem);
    std::unique_lock<std::mutex> ul(locks[get_stripe_index(hash_value)]);

    listref bucket = table[get_bucket_index(hash_value)];
    bucket.remove(elem);

    size--;
}

template<class T, class Hash>
bool striped_hash_set<T, Hash>::contains(const T& elem) const {
    size_t hash_value = Hash()(elem);
    std::unique_lock<std::mutex> ul(locks[get_stripe_index(hash_value)]);

    clistref bucket = table[get_bucket_index(hash_value)];

    return std::find(bucket.begin(), bucket.end(), elem) != bucket.end();
}

template<class T, class Hash>
size_t striped_hash_set<T, Hash>::get_bucket_index(size_t hash_value) const {
    return hash_value % table.size();
}

template<class T, class Hash>
size_t striped_hash_set<T, Hash>::get_stripe_index(size_t hash_value) const {
    return hash_value % locks.size();
}
