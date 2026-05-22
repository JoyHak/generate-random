#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <array>
using std::array;

TEST_CASE("integers") {
    constexpr size_t size = 5;
    auto arr = generate<array<int, size>>(size, 1, 10, 42);

    size_t count = 0;
    for (int val : arr) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("integer bools") {
    constexpr size_t size = 6;
    auto arr = generate_bool<array<int, size>>(size, 99);

    size_t count = 0;
    for (int val : arr) {
        REQUIRE((val == 0 || val == 1));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("pure bools") {
    constexpr size_t size = 6;
    auto arr = generate_bool<array<bool, size>>(size, 99);

    size_t count = 0;
    for (int val : arr) {
        REQUIRE((val == 0 || val == 1));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("floats") {
    constexpr size_t size = 4;
    auto arr = generate<array<double, size>>(size, 0.0, 1.0, 123);

    size_t count = 0;
    for (double val : arr) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("big integers") {
    constexpr size_t size = 10'000;
    auto arr = generate<array<int, size>>(size, 100'000, 500'000, 42);

    size_t count = 0;
    for (int val : arr) {
        REQUIRE(val >= 100'000);
        REQUIRE(val <= 500'000);
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("large integers") {
    constexpr size_t size = 10'000;
    using large = long long;
    auto arr = generate<array<large, size>>(size, -1'000'000, 1'000'000, 98765);

    size_t count = 0;
    for (large val : arr) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
        ++count;
    }
    REQUIRE(count == size);
}