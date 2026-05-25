#include "generate.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <unordered_set>  // faster because we don't need to preserve the order

TEST_CASE("collisions across random seeds", "[uniq_distribution][stress]") {
    using T = int;  // can be changed to short, long, etc.
    using uniq = rnd::detail::uniq_int_distribution<T>;
    using engine = std::mt19937;
    using set = std::unordered_set<T>;

    // [min, max] ranges
    auto min_vals = std::vector<T>{-100, -50, 0, 1};
    auto max_vals = std::vector<T>{1, 2, 5, 20, 120};

    constexpr int seeds_per_range = 50;

    for (const T& min_val : min_vals) {
        for (const T& max_val : max_vals) {
            if (max_val < min_val)
                continue;

            const size_t expected_count = static_cast<size_t>(max_val - min_val + 1);
            CAPTURE(min_val, max_val, expected_count);

            std::random_device rd;
            engine seed_gen(rd());

            for (int s = 0; s < seeds_per_range; ++s) {
                uint64_t random_seed = seed_gen();
                engine gen(random_seed);
                uniq dist(min_val, max_val);

                set values;
                values.reserve(expected_count);

                for (size_t i = 0; i < expected_count; ++i) {
                    values.insert(dist(gen));
                }

                CAPTURE(values.size(), expected_count, random_seed);
                REQUIRE(values.size() == expected_count);
            }
        }
    }
}

TEST_CASE("edge cases: small ranges", "[uniq_distribution][edge]") {
    using T = int;
    using uniq = rnd::detail::uniq_int_distribution<T>;
    using engine = std::mt19937;
    using set = std::unordered_set<T>;

    SECTION("Range [0, 1]") {
        uniq dist(0, 1);
        engine gen(12345);
        set s;
        s.insert(dist(gen));
        s.insert(dist(gen));
        REQUIRE(s.size() == 2);
    }

    SECTION("Range [5, 5]") {
        uniq dist(5, 5);
        engine gen(12345);
        set s;
        s.insert(dist(gen));
        REQUIRE(s.size() == 1);
        REQUIRE(*s.begin() == 5);
    }

    SECTION("Range [-1, 1]") {
        uniq dist(-1, 1);
        engine gen(67890);
        set s;
        s.insert(dist(gen));
        s.insert(dist(gen));
        s.insert(dist(gen));
        REQUIRE(s.size() == 3);
    }
}