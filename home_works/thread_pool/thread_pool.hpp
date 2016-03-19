#include "thread_pool.h"

template<typename T>
thread_pool<T>::thread_pool(): size(get_def_size()){

}

template<typename T>
thread_pool<T>::thread_pool(size_t size): size(size) {

}

template<typename T>
std::future<T> thread_pool<T>::submit(const std::function<T>& task) {

}

template<typename T>
size_t thread_pool<T>::getSize() const {
    return size;
}

template<typename T>
void thread_pool<T>::shutdown() {

}

template<typename T>
size_t thread_pool<T>::get_def_size() {
    size_t size = std::thread::hardware_concurrency();
    return size != 0 ? size : 4;
}
