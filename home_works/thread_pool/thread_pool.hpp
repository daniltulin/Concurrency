#include "thread_pool.h"

template<typename T>
thread_pool<T>::thread_pool(): workers(get_def_size()){
    launch_workers();
}

template<typename T>
thread_pool<T>::thread_pool(size_t size): workers(size) {
    launch_workers();
}

template<typename T>
void thread_pool<T>::launch_workers() {
}

template<typename T>
std::future<T> thread_pool<T>::submit(const std::function<T>& task) {
   package worker_package(task); 
   queue.push(worker_package);
   return worker_task.promise.get_future();
}

template<typename T>
thread_pool<T>::package(const std::function<T>& task): task(task) {

}

template<typename T>
size_t thread_pool<T>::getSize() const {
    return workers.size();
}

template<typename T>
void thread_pool<T>::shutdown() {
    should_shutdown = true;
}

template<typename T>
size_t thread_pool<T>::get_def_size() {
    size_t size = std::thread::hardware_concurrency();
    return size != 0 ? size : 4;
}
