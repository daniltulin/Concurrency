#include "cyclic_barrier.h"

cyclic_barrier::cyclic_barrier(size_t size): size(size) {

}

void cyclic_barrier::enter() {

    std::unique_lock<mutex_type> lock(mutex);
    size_t thread_era = era;
    threads_num++;

    if (threads_num == size) {
        era++;
        cv.notify_all();
        threads_num = 0;
        return;
    }

    cv.wait(lock, [&]() {
        return era != thread_era;
    });

}
