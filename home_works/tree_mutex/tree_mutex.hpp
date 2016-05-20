#include "tree_mutex.h"

void peterson_mutex::lock(size_t index) {

    want[index].store(true);
    victim.store(index);

    while(want[1 - index] && victim == index) {
        std::this_thread::yield();
    }
}

void peterson_mutex::unlock(size_t index) {
    want[index] = false;
}

void tree_mutex::lock(size_t index) {
    size_t id = tree.size() + index;

    while(id != 0) {
        tree[(id - 1) / 2].lock((id + 1) & 1);
        id = (id - 1) / 2;
    }
}

void tree_mutex::unlock(size_t index) {
    size_t depth = 0;

    while ((1 << depth) < num)
        depth++;

    size_t mutex_id = 0;

    for (int i = depth - 1; i >= 0; i--) {
        size_t bit = (index >> i) & 1;
        tree[mutex_id].unlock(bit);
        mutex_id = mutex_id * 2 + bit + 1;
    }
}
