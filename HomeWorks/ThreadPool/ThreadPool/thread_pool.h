#include <future>
#include <thread_safe_queue.hpp>

template<typename T>
class thread_pool {

public:

    thread_pool();
    thread_pool(size_t size);

    std::future<T> submit(const std::function<T>& task);

    inline size_t size() const;
    inline void shutdown();

private:

    size_t size;
    static size_t get_def_size();

};
