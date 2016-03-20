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
    for (auto it = workers.begin(); 
         it != workers.end();
         ++it) {
        *it = std::thread(thread_run, this);
    }
}

template<typename T>
void thread_run() {
    while(!should_shutdown) {
        worker_loop();
    }
}

template <typename T>
void worker_loop() {
    std::function<T> func;
    package worker_task;
    if (!queue.pop(worker_task))
        return;
    func = worker_task.task;
    auto value = func();
    worker_task.promise.set_value(value);
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
