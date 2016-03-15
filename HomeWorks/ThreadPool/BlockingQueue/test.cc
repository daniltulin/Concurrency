#include <iostream>
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
}
