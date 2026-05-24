#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <forward_list>

using std::forward_list,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    constexpr size_t size = 5;
    auto fwd = generate<forward_list<int>>(size, 1, 10, 42);

    size_t count = 0;
    for (int val : fwd) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("integer bools") {
    constexpr size_t size = 6;
    auto fwd = generate_bool<forward_list<int>>(size, 99);

    size_t count = 0;
    for (int val : fwd) {
        REQUIRE((val == 0 || val == 1));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("pure bools") {
    constexpr size_t size = 6;
    auto fwd = generate_bool<forward_list<bool>>(size, 99);

    size_t count = 0;
    for (int val : fwd) {
        REQUIRE((val == 0 || val == 1));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("floats") {
    constexpr size_t size = 4;
    auto fwd = generate<forward_list<double>>(size, 0.0, 1.0, 123);

    size_t count = 0;
    for (double val : fwd) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("big integers") {
    constexpr size_t size = 100'000;
    auto fwd = generate<forward_list<int>>(size, 100'000, 500'000, 42);

    size_t count = 0;
    for (int val : fwd) {
        REQUIRE(val >= 100'000);
        REQUIRE(val <= 500'000);
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("large integers") {
    constexpr size_t size = 100'000;
    using large = long long;
    auto fwd = generate<forward_list<large>>(size, -1'000'000, 1'000'000, 98765);

    size_t count = 0;
    for (large val : fwd) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
        ++count;
    }
    REQUIRE(count == size);
}

TEST_CASE("doubles, very large size") {
    auto fwd = generate<std::forward_list<double>>(1'000'000, 0.001, 0.999, 67890);

    for (double val : fwd) {
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
    }
}