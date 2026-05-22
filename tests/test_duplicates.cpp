#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
using std::vector, std::set, std::unordered_set;

TEST_CASE("unordered_set, no duplicates") {
    auto uset = generate<unordered_set<int>>(50, 1, 1000, 42);

    vector<int> vec(uset.begin(), uset.end());
    std::sort(vec.begin(), vec.end());

    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("unordered_set, detect manually") {
    auto uset = generate<unordered_set<int>>(100, 10, 50, 9999);

    std::vector<int> values;
    for (int val : uset) {
        values.push_back(val);
    }

    std::sort(values.begin(), values.end());

    bool has_duplicates = false;
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i] == values[i-1]) {
            has_duplicates = true;
            break;
        }
    }

    REQUIRE_FALSE(has_duplicates);
}

TEST_CASE("set, no duplicates") {
    auto s = generate<set<int>>(10, 1, 100, 123);

    vector<int> vec(s.begin(), s.end());
    std::sort(vec.begin(), vec.end());

    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("set, full range [1,10]") {
    auto s = generate<set<int>>(100, 1, 10); // more than uniq items

    REQUIRE(s.size() == 10);

    int expected = 1;
    for (int val : s) {
        REQUIRE(val == expected++);
    }
}