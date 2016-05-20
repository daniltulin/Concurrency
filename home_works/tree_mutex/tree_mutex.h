#include <thread>
#include <iostream>
#include <vector>
#include <atomic>
#include <array>
#include <cmath>

int two_pow(int n)
{
    int pow = 1;
    while(pow < (n + 1) / 2)
        pow *= 2;
    return pow;
}

class peterson_mutex {
public:

    peterson_mutex(): victim(0) {
        want[0] = false;
        want[1] = false;
    }

    void lock(size_t index);
    void unlock(size_t index);

private:

    std::array<std::atomic<bool>, 2> want;
    std::atomic<size_t> victim;

};

class tree_mutex {

public:

    tree_mutex(size_t n): num(n), tree(2 * two_pow(n) - 1) {}
    void lock(size_t index);
    void unlock(size_t index);

private:

    size_t num;
    std::vector<peterson_mutex> tree;

};
