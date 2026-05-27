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

TEST_CASE("characters") {
    constexpr short size = 5;
    auto s = generate<set<char>>(size, 'a', 'e');

    REQUIRE(s.size() == size);

    char prev = 0;
    for (char val : s) {
        REQUIRE(val >= 'a');
        REQUIRE(val <= 'e');
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
    constexpr size_t min = 1;
    constexpr size_t max = 1'000'000;

    auto s = generate<set<size_t>>(size, min, max, 98765);

    REQUIRE(s.size() == size);

    for (size_t val : s) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10;
    constexpr size_t min = 1;
    constexpr size_t max = std::numeric_limits<size_t>::max();

    auto s = generate<set<size_t>>(size, min, max, 98765);

    REQUIRE(s.size() == size);

    for (size_t val : s) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
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

TEST_CASE("negative range [-3,3]") {
    auto s = generate<set<int>>(10, -3, 3);

    REQUIRE(s.size() == 7);
    auto expected = std::vector<int>{-3, -2, -1, 0, 1, 2, 3};
    auto it = s.begin();
    for (int exp : expected) {
        REQUIRE(*it++ == exp);
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