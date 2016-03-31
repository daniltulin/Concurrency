#include <iostream>
#include <vector>
#include <thread>

#include "thread_pool.hpp"

#define BOOST_TEST_MODULE thread pool tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline) {
    thread_pool<int> pool;
    BOOST_TEST(pool.getSize() > 0);

    int value = 5;
    auto future = pool.submit([=](){
        return value;
    });
    BOOST_CHECKPOINT("is going to wait for future");
    BOOST_TEST(future.get() == value);
}

class test_fixture {

public:

    thread_pool<size_t> pool;
    size_t tasks_qty;

    test_fixture(): tasks_qty(10000) {

    }

    static size_t task(size_t begin, size_t end) {
        size_t sum = 0;
        for (size_t i = begin; i < end; ++i) {
            sum += i;
        }
        return sum;
    }

};

BOOST_FIXTURE_TEST_SUITE(testing, test_fixture)

BOOST_TEST_DECORATOR(*utf::timeout(10))
BOOST_AUTO_TEST_CASE(summing_test) {
    std::vector<std::shared_future<size_t>> futures(tasks_qty); 
    std::vector<std::pair<size_t, size_t>> ranges;

    for (auto it = futures.begin(); it != futures.end(); ++it) {
        size_t begin = 1, end = 2;
        auto func = std::bind(task, begin, end);
        *it = pool.submit(func);
        ranges.push_back({begin, end});
    }

    BOOST_TEST_CHECKPOINT("task are submitted");

    size_t one_thread_sum = 0, pool_sum  = 0;
    for (auto it = futures.begin();
         it != futures.end(); 
         ++it) {
        BOOST_TEST_CHECKPOINT("is going to get future at i=" << (it - futures.begin()));
        pool_sum += it->get();
    }

    BOOST_TEST_CHECKPOINT("got futures");

    for (auto it: ranges) {
        one_thread_sum += task(it.first, it.second);
    }

    BOOST_TEST(pool_sum == one_thread_sum);
}

BOOST_AUTO_TEST_SUITE_END()
