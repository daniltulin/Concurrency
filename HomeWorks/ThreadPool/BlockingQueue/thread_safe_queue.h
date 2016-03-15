#include <iostream>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <atomic>

template <typename T>
class thread_safe_queue {

public:

    thread_safe_queue(std::size_t capacity);
    thread_safe_queue(const thread_safe_queue&) = delete;

    bool enqueue(const T& item);
    bool pop(T& item);

    void shutdown();

private:

    std::deque<T> internal; 
    size_t capacity;

    std::atomic_bool should_shutdown;

    std::atomic_uint size;

    std::condition_variable enq_cv, pop_cv;
    std::mutex front_mutex, back_mutex;

};
