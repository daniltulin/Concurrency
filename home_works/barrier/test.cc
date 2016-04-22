#include <iostream>
#include <future>
#include "cyclic_barrier.hpp"

#define BOOST_TEST_MODULE library tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline, *utf::timeout(2)) {
    cyclic_barrier barrier(1);
    barrier.enter();
    barrier.enter();
    barrier.enter();
    BOOST_TEST_REQUIRE(true);
}
