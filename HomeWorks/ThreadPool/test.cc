#include <iostream>
#include <future>

#include "thread_pool.hpp"

#define BOOST_TEST_MODULE thread pool tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline) {
    BOOST_TEST(true);
}
