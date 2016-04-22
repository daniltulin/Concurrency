#include <condition_variable>
#include <mutex>

class cyclic_barrier {

public:

    cyclic_barrier(size_t size);
    void enter();

private:

    typedef std::mutex mutex_type;

    size_t size;
    size_t threads_num;

    size_t era;

    std::mutex mutex;
    std::condition_variable cv;

};
