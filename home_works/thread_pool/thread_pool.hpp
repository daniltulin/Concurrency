#include "thread_pool.h"
#include <limits>

namespace concurrency {

template<typename T>
thread_pool<T>::thread_pool(): thread_pool(get_def_size()) {

}

template<typename T>
thread_pool<T>::~thread_pool() {
    shutdown();
}

template<typename T>
thread_pool<T>::thread_pool(size_t size): workers(size), 
                                          queue(-1),
                                          should_shutdown(false) {
    launch_workers();
}

template<typename T>
void thread_pool<T>::launch_workers() {
    for (auto it = workers.begin(); 
         it != workers.end();
         ++it) {
        *it = std::thread(&thread_pool::thread_run, this);
    }
}

template<typename T>
void thread_pool<T>::thread_run() {
    while(!should_shutdown) {
        worker_loop();
    }
}

template <typename T>
void thread_pool<T>::worker_loop() {
    packaged_task<T> worker_task;
    if (!queue.pop(worker_task)) 
        return;
    auto value = worker_task.task();
    worker_task.promise.set_value(value);
}

template<typename T>
std::future<T> thread_pool<T>::submit(const std::function<T()>& task) {
    packaged_task<T> worker_packaged_task(task); 
    auto future = worker_packaged_task.promise.get_future();
    queue.enqueue(std::move(worker_packaged_task));
    return future;
}

template<typename T>
packaged_task<T>::packaged_task(const std::function<T()>& task): task(task) {

}

template<typename T>
packaged_task<T>::packaged_task(packaged_task<T>&& rhs): 
                               task(std::move(rhs.task)), 
                               promise(std::move(rhs.promise)) {

}

template<typename T>
packaged_task<T>& packaged_task<T>::operator=(packaged_task<T>&& rhs) {
    promise = std::move(rhs.promise);
    task = std::move(rhs.task);
    return *this;
}

template<typename T>
size_t thread_pool<T>::getSize() const {
    return workers.size();
}

template<typename T>
void thread_pool<T>::shutdown() {
    if (should_shutdown)
        return;
    queue.shutdown();
    should_shutdown = true;

    for (auto it = workers.begin();
         it != workers.end();
         ++it) {
        it->join();
    }
}

template<typename T>
size_t thread_pool<T>::get_def_size() {
    size_t size = std::thread::hardware_concurrency();
    return size != 0 ? size : 4;
}

}
