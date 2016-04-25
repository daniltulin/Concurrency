#include "cyclic_barrier.h"

cyclic_barrier::cyclic_barrier(size_t size): size(size),
                                             era(0),
                                             threads_num(0) {

}

void cyclic_barrier::enter() {

    std::unique_lock<mutex_type> lock(mutex);
    size_t thread_era = era;
    threads_num = threads_num + 1;

    if (threads_num == size) {
        era++;
        threads_num = 0;
        cv.notify_all();
        return;
    }

    cv.wait(lock, [&]() {
        return era != thread_era;
    });

}
