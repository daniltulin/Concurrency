#include "thread_safe_queue.h"

template <typename T>
thread_safe_queue<T>::thread_safe_queue(size_t c): capacity(c) {

}

template <typename T>
bool thread_safe_queue<T>::enqueue(const T& item) {
    std::lock_guard<std::mutex> locker(front_mutex);
    internal.push_front(item);
    size++;
    pop_cv.notify_one();
    return true;
}

template <typename T>
bool thread_safe_queue<T>::pop(T& item) {
    std::lock_guard<std::mutex> locker(back_mutex);
    pop_cv.wait(locker, [this](){return size == 0;});
    item = internal.back();
    internal.pop_back();
    return true;
}

template <typename T>
void thread_safe_queue<T>::shutdown() {

}
