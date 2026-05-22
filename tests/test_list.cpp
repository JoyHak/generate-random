#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <list>
using std::list;

TEST_CASE("integer bools") {
    auto lst = generate_bool<list<int>>(6, 99);

    REQUIRE(lst.size() == 6);
    for (int val : lst) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("pure bools") {
    auto lst = generate_bool<list<bool>>(6, 99);

    REQUIRE(lst.size() == 6);
    for (int val : lst) {
        REQUIRE((val == 0 || val == 1));
    }
}

TEST_CASE("long") {
    auto lst = generate<list<long>>(3, -5, 5, 111);

    REQUIRE(lst.size() == 3);
    for (long val : lst) {
        REQUIRE(val >= -5);
        REQUIRE(val <= 5);
    }
}

TEST_CASE("big integers") {
    constexpr size_t size = 100'000;
    auto lst = generate<list<int>>(size, 1, 1000, 42);

    REQUIRE(lst.size() == size);
    for (int val : lst) {
        REQUIRE(val >= 1);
        REQUIRE(val <= 1000);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10'000;
    auto lst = generate<list<long long>>(size, -1'000'000, 1'000'000, 98765);

    REQUIRE(lst.size() == size);
    for (long long val : lst) {
        REQUIRE(val >= -1'000'000);
        REQUIRE(val <= 1'000'000);
    }
}

TEST_CASE("floats, large size") {
    constexpr size_t size = 500'000;
    auto lst = generate<list<float>>(size, 0.1f, 10.0f, 12345);

    REQUIRE(lst.size() == size);
    for (float val : lst) {
        REQUIRE(val >= 0.1f);
        REQUIRE(val <= 10.0f);
    }
}

TEST_CASE("doubles, large size") {
    constexpr size_t size = 500'000;
    auto lst = generate<list<double>>(size, 0.001, 0.999, 67890);

    REQUIRE(lst.size() == size);
    for (double val : lst) {
        REQUIRE(val >= 0.001);
        REQUIRE(val <= 0.999);
    }
}

