#include <iostream>
#include <stack>
#include <mutex>
#include <memory>
#include <condition_variable>

std::stack<int> st;
std::mutex mut;

template <typename T>
class CMyStack {

public:

    CMyStack() {
    }

    void push(const T &obj) {
        std::unique_lock<std::mutex> lock(mutex);
        intern.push(obj);
        lock.unlock();
        cv.notify_one();
    }

    std::unique_ptr<T> wait_pop() {
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock_t());
        cv.wait(lock, [this]{ return intern.size() > 0; });
        return exclusive_pop();
    }


    bool try_pop(T &out) {
        std::unique_lock<std::mutex> lock(mutex);

        if (intern.size() > 0) {
            out = intern.top();
            intern.pop();
            return true;
        }

        return false;
    }


    size_t size() const;

private:

    std::stack<T> intern;
    std::mutex mutex;

    std::condition_variable cv; 

    std::unique_ptr<T> exclusive_pop() {
        std::unique_ptr<T> result(new T(intern.top()));
        intern.pop();
        return result;
    }
     

};

void f(int);

int main() {

    CMyStack<int> stack;

    return 0;
}
