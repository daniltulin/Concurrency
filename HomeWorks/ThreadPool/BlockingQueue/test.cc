#include <iostream>
#include <deque>
#include "thread_safe_queue.hpp"

#define BOOST_TEST_MODULE Blocking Queue tests
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
