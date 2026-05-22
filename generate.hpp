// <generate.hpp> -*- C++ -*-

// Copyright (C) 2026 Rafaello
//
// This file is part of the Personal Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef CPP_LIB_GENERATE_
#define CPP_LIB_GENERATE_
#include <random>
#include <type_traits>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template<typename T>
concept Integer = std::is_integral_v<T> && !std::is_same_v<T, bool>;

template<typename T>
concept Floating = std::is_floating_point_v<T>;

template<typename Container>
concept ContainerPush = requires(Container c, typename Container::value_type v) {
    c.push(v);
};

template<typename Container>
concept ContainerPushBack = requires(Container c, typename Container::value_type v) {
    c.push_back(v);
};

template<typename Container>
concept ContainerInteger = Integer<typename Container::value_type>;


template<Integer Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_int_distribution<Item>(min, max);
}

template<Floating Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_real_distribution<Item>(min, max);
}

inline auto make_distribution() {
    return std::bernoulli_distribution(0.5);
}

template<ContainerPush Container, typename Distribution, typename Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push(dist(gen));
    }
}

template<ContainerPushBack Container, typename Distribution, typename Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push_back(dist(gen));
    }
}


/**
 * @brief Generates a container of random values with the given distribution and generator.
 * <a href="https://cppinsights.io/s/93dcf68e">Insights</a>
 * @tparam Container The container type.
 * @tparam Item Container item type (auto-inferred).
 * @tparam Generator Random engine for sequence generation.
 * @param count Number of elements.
 * @param min Minimum value.
 * @param max Maximum value.
 * @param seed Initial seed value for the random number engine. Using the same seed allows reproducible results.
 * @return The filled container.
 */
template<
    typename Container,
    Arithmetic Item = typename Container::value_type,
    std::uniform_random_bit_generator Generator = std::mt19937
>
Container generate(
    size_t count = 10,
    Item min = 1, Item max = 26,
    std::random_device::result_type seed = std::random_device{}()
) {
    Container container;

    if constexpr (requires { container.reserve(count); }) {
        container.reserve(count);
    }

    Generator gen(seed);
    auto dist = make_distribution(min, max);
    fill_container(container, count, dist, gen);

    return container;
}

/**
 * @brief Generates a container of integer values 0/1 with the given distribution and generator.
 * @note The default distribution does not support the `bool` type, so this template uses integral type.
 * @tparam Container The container type.
 * @tparam Generator Random engine for sequence generation.
 * @param count Number of elements.
 * @param seed Initial seed value for the random number engine. Using the same seed allows reproducible results.
 * @return The filled container.
 */
template<
    typename Container,
    std::uniform_random_bit_generator Generator = std::mt19937
>
    Container generate_bool(
    size_t count = 10,
    std::random_device::result_type seed = std::random_device{}()
) {
    Container container;

    if constexpr (requires { container.reserve(count); }) {
        container.reserve(count);
    }

    Generator gen(seed);
    auto dist = make_distribution();
    fill_container(container, count, dist, gen);

    return container;
}
#endif  // CPP_LIB_GENERATE_
