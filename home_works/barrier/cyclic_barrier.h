#include <condition_variable>
#include <mutex>

class cyclic_barrier {

public:

    explicit cyclic_barrier(size_t size);
    cyclic_barrier(const cyclic_barrier&) = delete;

    void enter();

private:

    typedef std::mutex mutex_type;

    size_t size;
    size_t threads_num;

    size_t era;

    mutex_type mutex;
    std::condition_variable cv;

};
