#include <iostream>
#include <future>

#define BOOST_TEST_MODULE library tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline, *utf::timeout(2)) {
    int value = 5;
    auto future = std::async(std::launch::async, [=](){
        return value;
    });
    BOOST_TEST_REQUIRE(future.get() == value);
}

