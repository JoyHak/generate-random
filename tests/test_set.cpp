#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <set>
using std::set,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    constexpr size_t size = 5;
    auto s = generate<set<int>>(size, 1, 10, 42);

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
    auto s = generate_bool<set<int>>(6, 99);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("integer bools2") {
    auto s = generate<set<int>>(6, 0, 1);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("bools2") {
    auto s = generate<set<bool>>(6, 0, 1);

    REQUIRE(s.size() == 2);
    for (int val : s) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("1 bool") {
    auto s = generate_bool<set<bool>>(1, 99);

    REQUIRE(s.size() == 1);
    REQUIRE(s.contains(false));
}

TEST_CASE("2 bools") {
    auto s = generate_bool<set<bool>>(3, 99);

    REQUIRE(s.size() == 2);
    REQUIRE(s.contains(true));
    REQUIRE(s.contains(false));
}

TEST_CASE("floats") {
    constexpr size_t size = 4;
    auto s = generate<set<double>>(size, 0.1, 1.0, 123);

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
    auto s = generate<set<double>>(size, 0.001, 0.999, 67890);

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
    auto s = generate<set<int>>(size, 1, 1000, 42);

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
    auto s = generate<set<large>>(size, 1'000, 1'000'000, 98765);

    REQUIRE(s.size() == size);

    large prev = 0;
    for (large val : s) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
        REQUIRE(val != prev);
        prev = val;
    }
}

TEST_CASE("full range optimization [1,5]") {
    constexpr size_t count = 100; // more than 5
    auto s = generate<set<int>>(count, 1, 5);

    REQUIRE(s.size() == 5);
    auto it = s.begin();
    REQUIRE(*it++ == 1);
    REQUIRE(*it++ == 2);
    REQUIRE(*it++ == 3);
    REQUIRE(*it++ == 4);
    REQUIRE(*it++ == 5);
}

TEST_CASE("no full range optimization [0.0, 1.0]") {
    auto s = generate<set<double>>(1000, 0.0, 1.0);

    // No full range optimization for floating point
    REQUIRE(s.size() == 1000);

    double prev = 0.0;
    for (double val : s) {
        REQUIRE(val >= 0.0);
        REQUIRE(val <= 1.0);
        REQUIRE(val >= prev); // sorted
        prev = val;
    }
}

TEST_CASE("partial range [10,20]") {
    auto s = generate<set<int>>(5, 10, 20, 42);

    REQUIRE(s.size() == 5);
    for (int val : s) {
        REQUIRE(val >= 10);
        REQUIRE(val <= 20);
    }
}


TEST_CASE("large range [1,1000]") {
    auto s = generate<set<int>>(1000, 1, 1000);

    REQUIRE(s.size() == 1000);
    auto val = 1;
    for (int x : s) {
        REQUIRE(x == val++);
    }
}

TEST_CASE("negative range [-3,3]") {
    auto s = generate<set<int>>(10, -3, 3);

    REQUIRE(s.size() == 7);
    auto expected = std::vector<int>{-3, -2, -1, 0, 1, 2, 3};
    auto it = s.begin();
    for (int exp : expected) {
        REQUIRE(*it++ == exp);
    }
}

TEST_CASE("uint8_t full range [0,255]") {
    auto s = generate<set<uint8_t>>(1000, 0, 255);

    REQUIRE(s.size() == 256);
    int val = 0;
    for (uint8_t x : s) {
        REQUIRE(x == val++);
    }
}

TEST_CASE("floating range [0.1, 1.0]") {
    auto s = generate<set<double>>(5, 0.1, 1.0, 42);

    REQUIRE(s.size() == 5);
    for (double val : s) {
        REQUIRE(val >= 0.1);
        REQUIRE(val <= 1.0);
    }
}