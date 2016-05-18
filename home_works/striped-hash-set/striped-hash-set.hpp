#include "striped_hash_set.h"

template<class T, class Hash>
void striped_hash_set<T, Hash>::add(const T& elem) {

    size_t hash_value = Hash()(elem);
    std::unique_lock<std::mutex> ul(locks[get_stripe_index(hash_value)]);
    listref bucket = table[get_bucket_index(hash_value)];

    if(std::find(bucket.begin(), bucket.end(), elem) != bucket.end())
        return;

    bucket.push_front(elem);
    elementsNum++;

    double factor = static_cast<double>(elementsNum) / table.size();
    bool is_overload = (factor > = loadFactor); 

    if (is_overload) {
        size_t oldSize = table.size();
        ul.unlock();

        std::vector<std::unique_lock<std::mutex>> ulocks;
        if(oldSize == table.size()) {
            for(size_t i = 0; i < locks.size(); ++i)
                ulocks.emplace_back(locks[i]);
            auto oldTable = table;
            size_t size = growthFactor * table.size();
            table.clear();
            table.resize(size);
            for(const auto& buck : oldTable)
                for(const auto& elm : buck)
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
    elementsNum.fetch_sub(1);
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
