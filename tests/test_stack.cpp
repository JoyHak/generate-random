#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <stack>
using std::stack;

TEST_CASE("integers on stack") {
    auto stk = generate<stack<int>>(5, 10, 20, 42);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        int val = tmp.top();
        REQUIRE(val >= 10);
        REQUIRE(val <= 20);
        tmp.pop();
        ++count;
    }
    REQUIRE(count == 5);
}

TEST_CASE("bools on stack") {
    auto stk = generate_bool<stack<bool>>(6, 99);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        bool val = tmp.top();
        REQUIRE((val == 0 || val == 1));
        tmp.pop();
        ++count;
    }
    REQUIRE(count == 6);
}

TEST_CASE("big integers") {
    constexpr size_t size = 100'000;
    auto stk = generate<stack<int>>(size, 1, 1000, 42);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        int val = tmp.top();
        REQUIRE(val >= 1);
        REQUIRE(val <= 1000);
        tmp.pop();
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("large integers") {
    constexpr size_t size = 10'000;
    auto stk = generate<stack<long long>>(size, -1'000'000, 1'000'000, 98765);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        long long val = tmp.top();
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
        tmp.pop();
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("floats, large size") {
    constexpr size_t size = 500'000;
    auto stk = generate<stack<float>>(size, 0.1f, 10.0f, 12345);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        float val = tmp.top();
        REQUIRE(val >= 0.1f);
        REQUIRE(val <= 10.0f);
        tmp.pop();
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("doubles, large size") {
    constexpr size_t size = 500'000;
    auto stk = generate<stack<double>>(size, 0.001, 0.999, 67890);

    auto tmp = stk;
    int count = 0;
    while (!tmp.empty()) {
        double val = tmp.top();
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
        tmp.pop();
        ++count;
    }
    REQUIRE(count == size);
}