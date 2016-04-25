#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class CTATASLock {

public:

    CTATASLock(): counter(false) {}

    void lock() {
        while (true) {
            while(counter.load()) { }
            bool oldValue = false;
            if (counter.compare_exchange_weak(oldValue, true) && !oldValue) {
                break;
            }
        }
    }

    void unlock() {
        counter.store(false);
    }

    std::atomic_bool counter;
};

class CTASLock {
public:

    std::atomic_bool counter;

    CTASLock(): counter(false) {}

    void lock() {
        bool oldValue = false;
        while(!counter.compare_exchange_weak(oldValue, true) || oldValue) {
            oldValue = false;
        }
    }

    void unlock() {
        counter.store(false);
    }

};

template <typename T>
void test() {

    std::vector<std::thread> threads(3);

    auto t1 = std::chrono::high_resolution_clock::now();

    size_t value = 0;
    T lock;

    for (auto it = threads.begin(); it != threads.end(); ++it) {
        *it = std::thread([&]() {
            while(value < 10000000) {
                lock.lock();
                value++;
                lock.unlock();
            }
        });
    }

    for (auto it = threads.begin(); it != threads.end(); ++it) {
        it->join();
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
}

int main() {
    test<CTATASLock>();
    test<CTASLock>();
    return 0;
}
