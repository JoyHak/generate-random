#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <vector>

using std::vector,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

TEST_CASE("integers") {
    auto vec = generate<vector<int>>(5, 1, 10, 42);

    REQUIRE(vec.size() == 5);
    for (int val : vec) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 10);
    }
}

TEST_CASE("integer bools") {
    auto vec = generate_bool<vector<int>>(6, 99);

    REQUIRE(vec.size() == 6);
    for (int val : vec) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("pure bools") {
    auto vec = generate_bool<vector<bool>>(6, 99);

    REQUIRE(vec.size() == 6);
    for (int val : vec) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("floats") {
    auto vec = generate<vector<double>>(4, 0.0, 1.0, 123);

    REQUIRE(vec.size() == 4);
    for (double val : vec) {
        REQUIRE_THAT(val, Catch::Matchers::WithinAbs(0.5, 0.5));
    }
}

TEST_CASE("big integers") {
    constexpr size_t size = 100'000;
    auto vec = generate<vector<int>>(size, 1, 1000, 42);

    REQUIRE(vec.size() == size);
    for (int val : vec) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 1000);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10'000;
    auto vec = generate<vector<long long>>(size, -1'000'000, 1'000'000, 98765);

    REQUIRE(vec.size() == size);
    for (long long val : vec) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
    }
}

TEST_CASE("floats, large size") {
    constexpr size_t size = 500'000;
    auto vec = generate<vector<float>>(size, 0.1f, 10.0f, 12345);

    REQUIRE(vec.size() == size);
    for (float val : vec) {
        REQUIRE(val >= 0.1f);
        REQUIRE(val <= 10.0f);
    }
}

TEST_CASE("doubles, large size") {
    constexpr size_t size = 500'000;
    auto vec = generate<vector<double>>(size, 0.001, 0.999, 67890);

    REQUIRE(vec.size() == size);
    for (double val : vec) {
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
    }
}