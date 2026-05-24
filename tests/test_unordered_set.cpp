#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <unordered_set>

using std::unordered_set,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    constexpr size_t size = 5;
    auto s = generate<unordered_set<int>>(size, 1, 10, 42);

    REQUIRE(s.size() == size);

    int prev = 0;
    for (int val : s) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
        REQUIRE(val != prev);
        prev = val;
    }
}

TEST_CASE("integer bools") {
    auto s = generate_bool<unordered_set<int>>(6, 99);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("integer bools2") {
    auto s = generate<unordered_set<int>>(6, 0, 1);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("bools2") {
    auto s = generate<unordered_set<bool>>(6, 0, 1);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("1 bool") {
    auto s = generate_bool<unordered_set<bool>>(1, 99);

    REQUIRE(s.size() == 1);
    REQUIRE(s.contains(false));
}

TEST_CASE("2 bools") {
    auto s = generate_bool<unordered_set<bool>>(3, 99);

    REQUIRE(s.size() == 2);
    REQUIRE(s.contains(true));
    REQUIRE(s.contains(false));
}

TEST_CASE("bools iterator") {
    auto s = generate<unordered_set<int>>(6, 0, 1);

    REQUIRE(s.size() == 2);
    REQUIRE(s.find(0) != s.end());
    REQUIRE(s.find(1) != s.end());
}

TEST_CASE("full range [1,5]") {
    auto s = generate<unordered_set<int>>(100, 1, 5);

    REQUIRE(s.size() == 5);
    for (int val : s) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 5);
    }
}

TEST_CASE("partial insert [10,20]") {
    auto s = generate<unordered_set<int>>(5, 10, 20, 123);

    REQUIRE(s.size() == 5);
    for (int val : s) {
        REQUIRE(val >= 10);
        REQUIRE(val <= 20);
    }
}

TEST_CASE("floats") {
    constexpr size_t size = 4;
    auto s = generate<unordered_set<double>>(size, 0.1, 1.0, 123);

    REQUIRE(s.size() == size);

    double prev = 0;
    for (double val : s) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
        REQUIRE(val != prev);
        prev = val;
    }
}

TEST_CASE("doubles") {
    constexpr size_t size = 4;
    auto s = generate<unordered_set<double>>(size, 0.001, 0.999, 67890);

    REQUIRE(s.size() == size);

    double prev = 0;
    for (double val : s) {
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
        REQUIRE(val != prev);
        prev = val;
    }
}

TEST_CASE("big integers") {
    constexpr size_t size = 10;
    auto s = generate<unordered_set<int>>(size, 1, 1000, 42);

    REQUIRE(s.size() == size);

    int prev = 0;
    for (int val : s) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 1000);
        REQUIRE(val != prev);
        prev = val;
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10;
    using large = long long;
    auto s = generate<unordered_set<large>>(size, 1'000, 1'000'000, 98765);

    REQUIRE(s.size() == size);

    large prev = 0;
    for (large val : s) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
        REQUIRE(val != prev);
        prev = val;
    }
}