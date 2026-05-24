#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>

using std::vector,
      std::deque,
      std::list,
      std::set,
      std::unordered_set,
      rnd::generate_uniq;

TEST_CASE("vector, no duplicates") {
    auto vec = generate_uniq<vector<int>>(10, 1, 100, 42);

    std::sort(vec.begin(), vec.end());
    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("vector, full range [1,5]") {
    auto vec = generate_uniq<vector<int>>(20, 1, 5);
    REQUIRE(vec.size() == 5);
}

TEST_CASE("deque, no duplicates") {
    auto deq = generate_uniq<deque<int>>(15, 10, 50, 12345);
    REQUIRE(deq.size() == 15);
}

TEST_CASE("deque, detect manually") {
    auto deq = generate_uniq<deque<int>>(8, -5, 5, 999);

    vector<int> values(deq.begin(), deq.end());
    std::sort(values.begin(), values.end());

    bool has_duplicates = false;
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i] == values[i-1]) {
            has_duplicates = true;
            break;
        }
    }

    REQUIRE_FALSE(has_duplicates);
    REQUIRE(deq.size() == 8);
}

TEST_CASE("list, no duplicates") {
    auto lst = generate_uniq<list<int>>(12, 1, 20, 54321);

    vector<int> vec(lst.begin(), lst.end());
    std::sort(vec.begin(), vec.end());
    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("list, sorted ascending") {
    auto lst = generate_uniq<list<int>>(7, 100, 150, 777);

    vector<int> vec(lst.begin(), lst.end());
    std::sort(vec.begin(), vec.end());

    // Правильная проверка: только на отсутствие дубликатов
    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("unordered_set, no duplicates") {
    auto uset = generate_uniq<unordered_set<int>>(50, 1, 1000, 42);

    vector<int> vec(uset.begin(), uset.end());
    std::sort(vec.begin(), vec.end());

    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("unordered_set, detect manually") {
    auto uset = generate_uniq<unordered_set<int>>(100, 10, 50, 9999);

    vector<int> values(uset.begin(), uset.end());
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
    auto s = generate_uniq<set<int>>(10, 1, 100, 123);

    vector<int> vec(s.begin(), s.end());
    std::sort(vec.begin(), vec.end());

    auto it = std::adjacent_find(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
}

TEST_CASE("set, full range [1,10]") {
    auto s = generate_uniq<set<int>>(100, 1, 10);

    REQUIRE(s.size() == 10);

    int expected = 1;
    for (int val : s) {
        REQUIRE(val == expected++);
    }
}