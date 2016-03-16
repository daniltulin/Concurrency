#include <iostream>
#include <future>

#include "thread_safe_queue.hpp"

#define BOOST_TEST_MODULE safe thread queue tests
#include <boost/test/included/unit_test.hpp>

class baseline_fixture {

public:

    thread_safe_queue<int> queue;

    baseline_fixture(): queue(1000) {

    }

    ~baseline_fixture() {

    }

};

BOOST_FIXTURE_TEST_SUITE(baseline_tests, baseline_fixture)

BOOST_AUTO_TEST_CASE(baseline_test) {
    int value = 7;
    BOOST_TEST(queue.enqueue(value));
    int popped;
    BOOST_TEST(queue.pop(popped));
    BOOST_TEST(popped == value); 

    queue.enqueue(value);
    queue.shutdown();
    BOOST_TEST(queue.enqueue(value) == false);
    BOOST_TEST(queue.pop(popped) == true);
    BOOST_TEST(popped == value);
    BOOST_TEST(queue.pop(popped) == false);
}

BOOST_AUTO_TEST_CASE(one_thread_deque_test) {
    for (size_t i = 0; i < queue.get_capacity(); ++i) {
        BOOST_TEST(queue.enqueue(i));
    }

    int popped;
    for (size_t i = 0; i < queue.get_capacity(); ++i) {
        popped = 0;
        BOOST_TEST(queue.pop(popped));
        BOOST_TEST(popped == i);
    }
}

BOOST_AUTO_TEST_SUITE_END()

size_t summing(thread_safe_queue<int>& queue) {
    size_t sum = 0;
    int popped = 0;
    bool working = true;
    while(queue.pop(popped)) {
        sum += popped;
    }
    return sum;
}

BOOST_AUTO_TEST_CASE(summing_test) {
    size_t workers_qty = 4;
    std::vector<std::shared_future<size_t>> futures(workers_qty);
    size_t elements_qty = 200;
    thread_safe_queue<int> queue(elements_qty * workers_qty);
    size_t sum = 0;

    auto future = std::async(std::launch::async, summing, std::ref(queue));

    for (size_t i = 0; i < 10 * elements_qty * workers_qty; ++i) {
        sum += i;
        queue.enqueue(i);
    }
    queue.shutdown();
    BOOST_TEST(queue.enqueue(5) == false);

    size_t futures_sum = 0;
//    for (size_t i = 0; i < workers_qty; ++i) {
        futures_sum += future.get();
//    }
    BOOST_TEST(futures_sum == sum);
}
