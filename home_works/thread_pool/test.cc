#include <iostream>
#include <thread>

#include "thread_pool.hpp"

#define BOOST_TEST_MODULE thread pool tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline) {
    thread_pool<int> pool;
    BOOST_TEST(pool.getSize() > 0);
}

class test_fixture {

public:

    thread_pool<size_t> pool;
    size_t tasks_qty;

    thread_pool(): tasks_qty(200000) {

    }

    static size_t task(size_t begin, size_t end) {
        size_t sum = 0;
        for (size_t i = begin; i < end; ++i) {
            sum += i * i;
        }
        return sum;
    }

};

BOOST_FIXTURE_TEST_SUITE(test_fixture, summing_fixture)

BOOST_TEST_DECORATOR(*utf::timeout(5))
BOOST_DATA_TEST_CASE(summing_test, bdata::xrange(7)) {
    std::vector<std::future<size_t>> futures(tasks_qty); 
    std::vector<std::pair<size_t, size_t>> ranges;
    for (auto it = futures.begin(); it != futures.end(); ++it) {
        size_t begin = 1, end = 20;
        auto func = std::bind(task, begin, end);
        *it = pool.submit(func);
        ranges.pull_back({begin, end});
    }

    size_t one_thread_sum = 0, pool_sum  = 0;
    for (auto it = futures.begin(), range = ranges.begin(); 
         it != futures.end() && range != ranges.end(); 
         ++it, ++range) {
        pool_sum += it->get();
        one_thread_sum += task(range->first, range->second);
    }
    BOOST_TEST(pool_sum == one_thread_sum);
}

BOOST_AUTO_TEST_SUITE_END()
