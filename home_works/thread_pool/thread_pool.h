#include <future>
#include "thread_safe_queue.hpp"

template<typename T>
class thread_pool {

public:

    thread_pool();
    thread_pool(size_t size);

    std::future<T> submit(const std::function<T>& task);

    inline size_t getSize() const;
    inline void shutdown();

private:

    static size_t get_def_size();

    std::vector<std::thread> workers;
    std::atomic_bool should_shutdown;
    thread_safe_queue<package> queue;

    class package {
        package();
        std::promise<T> promise;
        std::function<T> task;
    };

    void launch_workers();
    void worker_loop();

};
