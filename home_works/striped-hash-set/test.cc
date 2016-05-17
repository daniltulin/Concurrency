#include <iostream>
#include <vector>
#include <thread>

#include "striped-hash-set.h"

#define BOOST_TEST_MODULE thread pool tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_TEST_DECORATOR(*utf::timeout(10))
BOOST_AUTO_TEST_CASE(baseline) {
    BOOST_TEST_REQUIRE(true);
}
