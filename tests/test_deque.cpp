#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <deque>

using std::deque,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    auto vec = generate<deque<int>>(5, 1, 10, 42);

    REQUIRE(vec.size() == 5);
    for (int val : vec) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
    }
}

TEST_CASE("integer bools") {
    auto lst = generate_bool<deque<int>>(6, 99);

    REQUIRE(lst.size() == 6);
    for (int val : lst) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("pure bools") {
    auto lst = generate_bool<deque<bool>>(6, 99);

    REQUIRE(lst.size() == 6);
    for (int val : lst) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("floats") {
    auto deq = generate<deque<double>>(4, 0.0, 1.0, 123);

    REQUIRE(deq.size() == 4);
    for (double val : deq) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
    }
}

TEST_CASE("big integers") {
    constexpr size_t size = 100'000;
    auto deq = generate<deque<int>>(size, 1, 1000, 42);

    REQUIRE(deq.size() == size);
    for (int val : deq) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 1000);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10'000;
    auto deq = generate<deque<long long>>(size, -1'000'000, 1'000'000, 98765);

    REQUIRE(deq.size() == size);
    for (long long val : deq) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
    }
}

TEST_CASE("floats, large size") {
    constexpr size_t size = 500'000;
    auto deq = generate<deque<float>>(size, 0.1f, 10.0f, 12345);

    REQUIRE(deq.size() == size);
    for (float val : deq) {
        REQUIRE(val >= 0.1f);
        REQUIRE(val <= 10.0f);
    }
}

TEST_CASE("doubles, large size") {
    constexpr size_t size = 500'000;
    auto deq = generate<deque<double>>(size, 0.001, 0.999, 67890);

    REQUIRE(deq.size() == size);
    for (double val : deq) {
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
    }
}

