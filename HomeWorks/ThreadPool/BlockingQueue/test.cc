#include <iostream>
#include <deque>
#include "thread_safe_queue.hpp"

#define BOOST_TEST_MODULE Blocking Queue tests
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(baseline_test) {
    int value = 7;
    thread_safe_queue<int> queue(10);
    BOOST_TEST(queue.enqueue(value));
    int popped;
    BOOST_TEST(queue.pop(popped));
    BOOST_TEST(popped == value); 

    queue.enqueue(value);
    queue.shutdown();
    BOOST_TEST(queue.pop(popped) == false);
}

BOOST_AUTO_TEST_CASE(one_thread_deque_test) {
    size_t size = 1000;
    thread_safe_queue<int> queue(size);
    for (size_t i = 0; i < size; ++i) {
        BOOST_TEST(queue.enqueue(i));
    }

    int popped;
   for (size_t i = 0; i < size; ++i) {
        popped = 0;
        BOOST_TEST(queue.pop(popped));
        BOOST_TEST(popped == i);
    }
}
