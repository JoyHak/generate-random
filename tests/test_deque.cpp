#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <deque>

using std::deque,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    auto deq = generate<deque<int>>(5, 1, 10, 42);

    REQUIRE(deq.size() == 5);
    for (int val : deq) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
    }
}

TEST_CASE("characters") {
    auto deq = generate<deque<char>>(5, 'a', 'e');

    REQUIRE(deq.size() == 5);
    for (char val : deq) {
        REQUIRE(val >= 'a');
        REQUIRE(val <= 'e');
    }
}

TEST_CASE("integer bools") {
    auto deq = generate_bool<deque<int>>(6, 99);

    REQUIRE(deq.size() == 6);
    for (int val : deq) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("pure bools") {
    auto deq = generate_bool<deque<bool>>(6, 99);

    REQUIRE(deq.size() == 6);
    for (int val : deq) {
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
    constexpr size_t size = 10;
    constexpr size_t min = std::numeric_limits<size_t>::min();
    constexpr size_t max = 1'000'000;

    auto deq = generate<deque<size_t>>(size, min, max, 98765);

    REQUIRE(deq.size() == size);

    for (size_t val : deq) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10;
    constexpr size_t min = std::numeric_limits<size_t>::min();
    constexpr size_t max = std::numeric_limits<size_t>::max();

    auto deq = generate<deque<size_t>>(size, min, max, 98765);

    REQUIRE(deq.size() == size);

    for (size_t val : deq) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
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

