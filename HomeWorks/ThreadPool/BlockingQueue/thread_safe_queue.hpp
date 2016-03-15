#include "thread_safe_queue.h"

template <typename T>
thread_safe_queue<T>::thread_safe_queue(size_t c): capacity(c) {

}

template <typename T>
bool thread_safe_queue<T>::enqueue(const T& item) {
    std::unique_lock<std::mutex> locker(front_mutex);

    enq_cv.wait(locker, [this](){return size <= capacity || should_shutdown == false;});

    if (should_shutdown)
        return false;

    internal.push_front(item);
    size.store(size + 1);
    if (size == 1)
        pop_cv.notify_one();

    return true;
}

template <typename T>
bool thread_safe_queue<T>::pop(T& item) {
    std::unique_lock<std::mutex> locker(back_mutex);

    pop_cv.wait(locker, [this](){return size > 0 || should_shutdown == false;});

    if (should_shutdown)
        return false;

    item = internal.back();
    internal.pop_back();

    size.store(size - 1);
    if (size == capacity - 1)
        enq_cv.notify_one();

    return true;
}

template <typename T>
void thread_safe_queue<T>::shutdown() {
    should_shutdown.store(true);
    pop_cv.notify_all();
    enq_cv.notify_all();
}
