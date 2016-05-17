#include <iostream>
#include <future>
#include <thread_safe_queue.hpp>
#include <atomic>
#include "cyclic_barrier.hpp"

#define BOOST_TEST_MODULE barrier tests
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(baseline, *utf::timeout(2)) {
    cyclic_barrier barrier(1);
    barrier.enter();
    barrier.enter();
    barrier.enter();
    BOOST_TEST_REQUIRE(true);
}

class test_fixture {

public:

    test_fixture(): workers_num(500),
                    value(0) {

    }

    size_t workers_num;

    std::atomic_int value;
};

BOOST_FIXTURE_TEST_SUITE(testing, test_fixture)

BOOST_TEST_DECORATOR(*utf::timeout(1))
BOOST_AUTO_TEST_CASE(three_workers_test) {
    cyclic_barrier barrier(3);

    auto first = std::thread([&] {
        barrier.enter();
    });

    auto second = std::thread([&] {
        barrier.enter();
    });

    barrier.enter();

    first.join();
    second.join();

    BOOST_TEST_REQUIRE(true);
}

BOOST_TEST_DECORATOR(*utf::timeout(4))
BOOST_AUTO_TEST_CASE(increment_case) {
    std::vector<std::thread> threads(500);
    cyclic_barrier barrier(4);

    for (auto &it: threads) {
        it = std::thread([&] {
            barrier.enter();
            value++;
        });
    }

    BOOST_TEST_CHECKPOINT("is going to get futures");
    for (auto &it: threads) {
        it.join();
    }

    BOOST_TEST_CHECKPOINT("is going to shutdown");
    BOOST_TEST_REQUIRE(value == 500);
}
BOOST_AUTO_TEST_SUITE_END()

class queue_fixture_test {

public:

    queue_fixture_test(): queue(-1), barrier(4) {

    }

    thread_safe_queue<int> queue;
    cyclic_barrier barrier;

};

BOOST_FIXTURE_TEST_SUITE(queue_testing, queue_fixture_test)


BOOST_AUTO_TEST_SUITE_END()
