#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <list>

using std::list,
      rnd::generate_bool,
      rnd::generate_uniq,
      rnd::generate;

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
    for (bool val : lst) {
        REQUIRE((val == true || val == false));
    }
}

TEST_CASE("characters") {
    auto lst = generate<list<char>>(5, 'a', 'e');

    REQUIRE(lst.size() == 5);
    for (char val : lst) {
        REQUIRE(val >= 'a');
        REQUIRE(val <= 'e');
    }
}

TEST_CASE("big integers") {
    constexpr size_t size = 10;
    constexpr size_t min = std::numeric_limits<size_t>::min();
    constexpr size_t max = 1'000'000;

    auto lst = generate<list<size_t>>(size, min, max, 98765);

    REQUIRE(lst.size() == size);

    for (size_t val : lst) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
    }
}

TEST_CASE("large integers") {
    constexpr size_t size = 10;
    constexpr size_t min = std::numeric_limits<size_t>::min();
    constexpr size_t max = std::numeric_limits<size_t>::max();

    auto lst = generate<list<size_t>>(size, min, max, 98765);

    REQUIRE(lst.size() == size);

    for (size_t val : lst) {
        REQUIRE(val >= min);
        REQUIRE(val <= max);
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

