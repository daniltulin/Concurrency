#include <iostream>
#include <forward_list>
#include <condition_variable>
#include <mutex>

template <typename T>
class thread_safe_queue {

public:

    thread_safe_queue(std::size_t capacity);
    thread_safe_queue(const thread_safe_queue&) = delete;

    bool enqueue(const T& item);
    bool pop(T& item);

    void shutdown();

private:

    std::forward_list<T> internal; 
    size_t capacity;

    size_t size;

    std::condition_variable enq_cv, pop_cv;
    std::mutex front_mutex, back_mutex;

};
