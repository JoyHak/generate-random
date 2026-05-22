#include "generate.hpp"
#include "icecream.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <deque>
#include <list>


TEST_CASE("integer containers")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto vec = generate<std::vector<int>>(5, 1, 10, 12345);
        IC(vec);

        REQUIRE(str == "ic| vec: [3, 6, 2, 5, 10]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto deq = generate<std::deque<int>>(3, -5, 5, 12345);
        IC(deq);

        REQUIRE(str == "ic| deq: [-3, 0, -4]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto lst = generate<std::list<int>>(4, 100, 200, 12345);
        IC(lst);

        REQUIRE(str == "ic| lst: [198, 129, 101, 136]\n");
    }
}

TEST_CASE("floating point containers")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto vec = generate<std::vector<float>>(3, 0.1f, 0.9f, 12345);
        IC(vec);

        REQUIRE(str == "ic| vec: [0.84369284, 0.8121238, 0.35310042]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto deq = generate<std::deque<float>>(2, 10.5, 20.5, 12345);
        IC(deq);

        REQUIRE(str == "ic| deq: [19.401546, 11.807073]\n");
    }
}


TEST_CASE("bool containers")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto vec = generate_bool<std::vector<short>>(5, 12345);
        IC(vec);

        REQUIRE(str == "ic| vec: [0, 1, 1, 0, 0]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto deq = generate_bool<std::deque<short>>(4, 12345);
        IC(deq);

        REQUIRE(str == "ic| deq: [0, 1, 1, 0]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto lst = generate_bool<std::list<short>>(6, 12345);
        IC(lst);

        REQUIRE(str == "ic| lst: [0, 1, 1, 0, 0, 0]\n");
    }
}

TEST_CASE("default parameters")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto vec = generate<std::vector<int>>();
        IC(vec);

        REQUIRE(str != "ic| vec: []\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto vec = generate_bool<std::vector<short>>();
        IC(vec);

        REQUIRE(str != "ic| vec: []\n");
    }
}

TEST_CASE("container content consistency")
{
    // Using fixed seed for reproducible results
    const auto seed = 12345u;

    SECTION("vector<int> with range [1, 5]")
    {
        auto vec = generate<std::vector<int>>(5, 1, 5, seed);
        REQUIRE(vec.size() == 5);
        for (const auto& val : vec)
        {
            REQUIRE(val >= 1);
            REQUIRE(val <= 5);
        }
    }

    SECTION("deque<float> with range [0.1, 0.5]")
    {
        auto deq = generate<std::deque<float>>(4, 0.1f, 0.5f, seed);
        REQUIRE(deq.size() == 4);
        for (const auto& val : deq)
        {
            REQUIRE(val >= 0.1f);
            REQUIRE(val <= 0.5f);
        }
    }
}

TEST_CASE("empty container")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    auto vec = generate<std::vector<int>>(0, 1, 10, 12345);
    IC(vec);

    REQUIRE(str == "ic| vec: []\n");
}

TEST_CASE("single element container")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    auto vec = generate<std::vector<int>>(1, 42, 42, 12345);
    IC(vec);

    REQUIRE(str == "ic| vec: [42]\n");
}