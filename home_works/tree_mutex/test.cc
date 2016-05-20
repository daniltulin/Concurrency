#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include "tree_mutex.hpp"

#define BOOST_TEST_MODULE peterson tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_TEST_DECORATOR(*utf::timeout(10))
BOOST_AUTO_TEST_CASE(baseline) {

    size_t qty = 10;

    tree_mutex mutex(qty);
    std::vector<std::thread> threads;

    size_t counter = 0;

    for(size_t idx = 0; idx < qty; ++idx) {
        threads.emplace_back([&]() {
            int index = idx;
            mutex.lock(index);
            for (int i = 0; i < 100; ++i) {
                counter++;
            }
            mutex.unlock(index);
        });
    }

    for(auto& thread: threads) {
        thread.join();
    }

    BOOST_TEST_REQUIRE(counter == qty * 100);
}
