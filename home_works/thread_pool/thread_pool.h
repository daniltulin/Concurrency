#include <future>
#include "thread_safe_queue.hpp"

namespace concurrency {

template<typename T>
struct packaged_task {
    packaged_task() = default;
    packaged_task(const std::function<T()>& task);
    packaged_task(packaged_task&& rhs);
    packaged_task& operator=(packaged_task&& rhs);
    std::promise<T> promise;
    std::function<T()> task;
};


template<typename T>
class thread_pool {

public:

    thread_pool();
    ~thread_pool();
    thread_pool(size_t size);

    std::future<T> submit(const std::function<T()>& task);

    inline size_t getSize() const;
    inline void shutdown();

private:

    static size_t get_def_size();

    std::vector<std::thread> workers;
    std::atomic_bool should_shutdown;
    thread_safe_queue<packaged_task<T>> queue;

    void launch_workers();

    void thread_run();
    inline void worker_loop();

};

}

using concurrency::thread_pool;
