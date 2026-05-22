#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <deque>
#include <list>
#include <stack>


TEST_CASE("integers in vector") {
    auto vec = generate<std::vector<int>>(5, 1, 10, 42);

    REQUIRE(vec.size() == 5);
    for (int val : vec) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
    }
}

TEST_CASE("floats in deque") {
    auto deq = generate<std::deque<double>>(4, 0.0, 1.0, 123);

    REQUIRE(deq.size() == 4);
    for (double val : deq) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
    }
}

TEST_CASE("bools in list") {
    auto lst = generate_bool<std::list<int>>(6, 99);

    REQUIRE(lst.size() == 6);
    for (int val : lst) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("Stack functionality") {
    std::stack<int> stk;
    auto gen_stk = generate<std::stack<int>>(5, 10, 20, 42);

    auto temp = gen_stk;
    int count = 0;
    while (!temp.empty()) {
        int val = temp.top();
        REQUIRE(val >= 10);
        REQUIRE(val <= 20);
        temp.pop();
        ++count;
    }
    REQUIRE(count == 5);
}

TEST_CASE("List functionality") {
    auto lst = generate<std::list<long>>(3, -5, 5, 111);

    REQUIRE(lst.size() == 3);
    for (long val : lst) {
        REQUIRE(val >= -5);
        REQUIRE(val <= 5);
    }
}

TEST_CASE("Reserve functionality") {
    auto vec = generate<std::vector<float>>(100, 0.0f, 1.0f, 555);

    REQUIRE(vec.capacity() >= 100);
}