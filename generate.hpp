/*
 * <generate.hpp> -*- C++ -*-
 * 
 * Copyright (C) 2026 - present, Rafaello
 * <https://github.com/JoyHak/generate-random>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#ifndef GENERATE_HPP_
#define GENERATE_HPP_
#include <random>
#include <concepts>

/**
 * According to the <a href="https://eel.is/c++draft/rand#req.genl-1">standart</a>:
 * <br><i>where the template parameters are not constrained, the names of template parameters
 * are used to express type requirements on an instantiated template T</i>.<br><br>
 *
 * The standard does not describe <i>concepts</i> that would allow efficient type deduction for wrappers.
 * So we will have to create our own <i>concepts</i> that use internal functions from
 * <a href="https://github.com/llvm/llvm-project/blob/8cc944cb29cec7974509a34ae8ad85c483cbd120/libcxx/include/__random/is_valid.h#L47">LLVM Clang</a>.
 * They will help the compiler choose template specifications that allow generating
 * containers of any type: `char, bool, string`, ... despite the restrictions of distributions.
 */

template<typename T>
concept Integer = std::__libcpp_random_is_valid_inttype<T>::value;

template<typename T>
concept Floating = std::__libcpp_random_is_valid_realtype<T>::value;

template<typename T>
concept Arithmetic = Integer<T> || Floating<T>;

template<typename Cont>
concept ContainerPush = requires(Cont c, typename Cont::value_type v) {
    c.push(v);
};

template<typename Cont>
concept ContainerPushBack = requires(Cont c, typename Cont::value_type v) {
    c.push_back(v);
};

template<typename Cont>
concept Array = requires(Cont c, typename Cont::value_type v) {
    c.fill(v);
    { c.at(0) } -> std::convertible_to<typename Cont::value_type&>;
    // { c[0] } -> std::convertible_to<typename Cont::value_type&>;  // less useful than .at()
};

template<typename Gen>
concept UniformGenerator = std::uniform_random_bit_generator<Gen>;

// ── Distributions ────────────────────────────────────────────────────────────────────

template<Integer Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_int_distribution<Item>(min, max);
}

template<Floating Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_real_distribution<Item>(min, max);
}

inline auto make_distribution() {
    // 50% true, 50% false
    return std::bernoulli_distribution(0.5);
}

// ── Fillers ──────────────────────────────────────────────────────────────────────────

template<ContainerPush Container, typename Distribution, UniformGenerator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push(dist(gen));
    }
}

template<ContainerPushBack Container, typename Distribution, UniformGenerator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push_back(dist(gen));
    }
}

template<Array Container, typename Distribution, UniformGenerator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.at(i) = dist(gen);
    }
}

// ── Generators ───────────────────────────────────────────────────────────────────────

/**
 * @brief Generates a container of random values with the given distribution and generator.
 * <a href="https://cppinsights.io/s/93dcf68e">Insights</a>
 * @tparam Container The container type.
 * @tparam Item Container item type (auto-inferred).
 * @tparam Generator Random engine for sequence generation.
 * @tparam Seed Unsigned integer type.
 * @param count Number of elements.
 * @param min Minimum value.
 * @param max Maximum value.
 * @param seed Initial seed value for the random number engine. Using the same seed allows reproducible results.
 * @return The filled container.
 */
template<
    typename Container,
    Arithmetic Item = typename Container::value_type,
    UniformGenerator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
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
 * @tparam Seed Unsigned integer type.
 * @param count Number of elements.
 * @param seed Initial seed value for the random number engine. Using the same seed allows reproducible results.
 * @return The filled container.
 */
template<
    typename Container,
    UniformGenerator Generator = std::mt19937,
    typename Seed = unsigned int
>
    Container generate_bool(
    size_t count = 10,
    Seed seed = std::random_device{}()
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
#endif  // GENERATE_HPP_
