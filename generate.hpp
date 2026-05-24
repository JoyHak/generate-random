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

namespace rnd {

using std::integral, std::floating_point;

template<typename Gen>
concept uniform_generator = std::uniform_random_bit_generator<Gen>;

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
concept rand_integer = std::__libcpp_random_is_valid_inttype<T>::value;

template<typename T>
concept rand_floating = std::__libcpp_random_is_valid_realtype<T>::value;

template<typename T>
concept rand_arithmetic = rand_integer<T> || rand_floating<T>;

template<typename T>
concept boolean = std::is_same_v<bool, T>;

/**
 * @tparam Cont queue, stack, ...
 */
template<typename Cont>
concept container_push = requires(Cont c, typename Cont::value_type v) {
    c.push(v);
};

/**
 * @tparam Cont list, deque, vector, ...
 */
template<typename Cont>
concept container_push_back = requires(Cont c, typename Cont::value_type v) {
    c.push_back(v);
};

/**
 * @tparam Cont forward_list, ...
 */
template<typename Cont>
concept container_push_front = requires(Cont c, typename Cont::value_type v) {
    c.push_front(v);
};

template<typename Cont>
concept container_insert = requires(Cont c, typename Cont::value_type v) {
    c.insert(v);
};

/**
 * @tparam Cont deque, ... (two-way container)
 * @note Helper to choose between push_back() and push_front()
 */
template<typename Cont>
concept container_front_back = container_push_front<Cont> && container_push_back<Cont>;

/**
 * @tparam Cont list, ... (list with fast insertion)
 * @note Helper to choose between push_back(), push_front() and insert()
 */
template<typename Cont>
concept list = container_push_front<Cont> && container_push_back<Cont> && container_insert<Cont>;

/**
 * @tparam Cont array, ...
 */
template<typename Cont>
concept array = requires(Cont c, typename Cont::value_type v) {
    c.fill(v);
    { c.at(0) } -> std::convertible_to<typename Cont::value_type&>;
    // { c[0] } -> std::convertible_to<typename Cont::value_type&>;  // less useful than .at()
};

/**
 * Associative containers with unique values <i>(set, map, ...)</i>
 * will be generated differently than containers that support duplicates.
 * Since uniform distribution is not suitable for unique values due to high probability
 * of duplicating random values, we'll use custom distribution for containers with unique values
 * <i>(except for <code>multimap, multiset</code> etc., see below)</i><br><br>
 *
 * The <a href="https://eel.is/c++draft/associative.reqmts">standart</a> does not describe
 * clear requirements for each associative container:
 * <code> set, unordered_set, map, unordered_map</code>, etc.<br>
 * In order to distinguish such containers from each other, we will create concepts based on
 * public <i>typedefs</i>.
 */

/**
 * @tparam Cont set, unordered_set, map, unordered_map, ...
 * @remark <code>multimap, multiset</code> etc. do not store unique elements.<br>
 * Actually they do not fit this concept, since their <i>insert()</i> method
 * does not return <i>std::pair</i>.<br><br>
 * All concepts below will not involve these containers.
 * Random values for <code>multimap, multiset</code> etc. will be generated in the same way as for other containers
 * using uniform distribution.
 */
template<typename Cont>
concept associative_container =
    container_insert<Cont>
 && requires(Cont c, typename Cont::value_type v) {
        typename Cont::key_type;
        { c.insert(v) } -> std::same_as<
            std::pair<typename Cont::iterator,
            bool>
        >;
    };

template<typename Cont>
concept unordered_set =
    associative_container<Cont>
 && requires {
        typename Cont::key_equal;
        typename Cont::hasher;
    };

template<typename Cont>
concept ordered_set =
    associative_container<Cont>
 && requires {
        typename Cont::key_compare;
        typename Cont::value_compare;
    };

template<typename Cont>
concept hash_table =
    associative_container<Cont>
 && requires {
        typename Cont::key_equal;
        typename Cont::mapped_type;
    };

template<typename Cont>
concept hash_map =
    associative_container<Cont>
    && requires {
        typename Cont::key_compare;
        typename Cont::mapped_type;
    };

template<typename Cont>
concept reversible_container = !associative_container<Cont>;

// ── Fillers ──────────────────────────────────────────────────────────────────────────

namespace detail {

/**
 * @tparam Container array, ...
 */
template<array Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.at(i) = dist(gen);
    }
}

/**
 * @tparam Container queue, stack, ...
 */
template<container_push Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push(dist(gen));
    }
}

/**
 * @tparam Container deque, ... (two-way container)
 * @note Helper to choose between push_back() and push_front()
 */
template<container_front_back Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push_back(dist(gen));
    }
}

/**
 * @tparam Container list, deque, vector, ...
 */
template<container_push_back Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push_back(dist(gen));
    }
}

/**
 * @tparam Container forward_list, ...
 */
template<container_push_front Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.push_front(dist(gen));
    }
}

/**
 * @tparam Container list, ... (list with fast insertion)
 * @note Helper to choose between push_back(), push_front() and insert()
 */
template<list Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.insert(dist(gen));
    }
}

/**
 * @tparam Container set, unordered_set, ...
 */
template<container_insert Container, typename Distribution, uniform_generator Generator>
inline void fill_container(Container& container, size_t count, Distribution& dist, Generator& gen) {
    for (size_t i = 0; i < count; ++i) {
        container.insert(dist(gen));
    }
}

// ── Distributions ────────────────────────────────────────────────────────────────────

template<integral T>
class uniq_distribution {
 private:
    T min_val;
    T count;
    T max_count;
    bool order;
    bool seed_initialized;

 public:
    uniq_distribution(T min, T max)
      : min_val(min), count(0),
        max_count(max - min + 1),
        order(false), seed_initialized(false) {}

    template<std::uniform_random_bit_generator Generator>
    T operator()(Generator& gen) {
        if (!seed_initialized) {
            std::uniform_int_distribution<T> dist;
            order = dist(gen) & 1;
            seed_initialized = true;
        }

        if (max_count == 1) {
            return min_val;
        }
        if (max_count == 2) {
            return min_val + (order ^ (count++ & 1));
        }

        T x = count++;
        x ^= static_cast<T>(order) * 31337;
        x ^= x >> 16;
        x *= 0x7feb352d;
        x ^= x >> 15;
        x *= 0x846ca68b;
        x ^= x >> 16;

        return min_val + (x % max_count);
    }
};

template<integral Item>
inline auto make_uniq_distribution(Item min, Item max) {
    return uniq_distribution<Item>(min, max);
}

template<rand_integer Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_int_distribution<Item>(min, max);
}

template<rand_floating Item>
inline auto make_distribution(Item min, Item max) {
    return std::uniform_real_distribution<Item>(min, max);
}

inline auto make_distribution() {
    // 50% true, 50% false
    return std::bernoulli_distribution(0.5);
}

// ── Helpers ──────────────────────────────────────────────────────────────────────────

/**
 * Limits the count of elements generated in a container if the upper bound `max` is lower than `count`.
 * @tparam Item The type of element in the container to be generated.
 * The function requires support for comparison and subtraction operations,
 * so it only works with <i>int</i> types.
 * @param count Number of elements to generate.
 * @param min Minimum value.
 * @param max Maximum value.
 */
template<integral Item>
inline size_t clamp_count(size_t count, Item min = 1, Item max = 26) {
    uint64_t min_val = static_cast<uint64_t>(min);
    uint64_t max_val = static_cast<uint64_t>(max);
    uint64_t total_unique = max_val - min_val + 1;

    if (count >= total_unique) {
        count = static_cast<size_t>(total_unique);
    }

    return count;
}

template<typename Item>
inline size_t clamp_count(size_t count, Item, Item) {
    return count;
}

} // detail

// ── Generators ───────────────────────────────────────────────────────────────────────

/**
 * @brief Generates a container of unique random integer values.
 * @remark <code>set, unordered_set, map, unordered_map</code> and other similar assoc. containers excludes duplicates.
 * If the generated value already exists, <strong>it will not be added!</strong><br><br>
 * If count > max, the final container size will be max - min + 1. Otherwise it will be equal to count.
 * @tparam Container The container type.
 * @tparam Item Integer container item type (auto-inferred).
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
    integral Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate_uniq(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
) {
    Container container;

    count = detail::clamp_count<Item>(count, min, max);
    if constexpr (requires { container.reserve(count); }) {
        container.reserve(count);
    }

    Generator gen(seed);
    auto dist = detail::make_uniq_distribution(min, max);
    detail::fill_container(container, count, dist, gen);

    return container;
}

/**
 * @brief Generates a container of unique random floating point values.
 * @tparam Container The container type.
 * @tparam Item Floating point container item type (auto-inferred).
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
    rand_floating Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate_uniq(
    size_t count = 10,
    Item min = 1.0f, Item max = 26.0f,
    Seed seed = std::random_device{}()
) {
    Container container;

    if constexpr (requires { container.reserve(count); }) {
        container.reserve(count);
    }

    Generator gen(seed);
    auto dist = detail::make_distribution(min, max);
    detail::fill_container(container, count, dist, gen);

    return container;
}

/**
 * @brief Generates a container of random boolean values.
 * Allows any type of container element that can be cast to 0 or 1.
 * @note The default distribution does not support the `bool` type, so this template uses integral type.
 * @tparam Container The container type.
 * @tparam Generator Random engine for sequence generation.
 * @tparam Seed Unsigned integer type.
 * @param count Number of elements.
 * @param seed Initial seed value for the random number engine. Using the same seed allows reproducible results.
 * @return The filled container.
 */
template<
    reversible_container Container,
    uniform_generator Generator = std::mt19937,
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
    auto dist = detail::make_distribution();
    detail::fill_container(container, count, dist, gen);

    return container;
}

/**
 * @brief Generates an associative container of random boolean values.
 * @note The default distribution does not support the `bool` type, so this template uses integral type.
 * @tparam Container The container type.
 * @tparam Generator Random engine for sequence generation.
 * @tparam Seed Unsigned integer type.
 * @param count Size limit (1-2).
 * @param seed Initial seed value to mix booleans oreder (0, 1) or (1, 0).
 * @return The filled container.
 */
template<
    associative_container Container,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate_bool(
    size_t count,
    Seed seed = std::random_device{}()
) {
    Container container;

    Generator gen(seed);
    auto dist = detail::make_distribution();

    bool value = dist(gen);
    container.insert(value);

    if (count > 1) {
        container.insert(value == true ? false : true);
    }

    return container;
}

/**
 * @brief Generates a container of random values.
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
    reversible_container Container,
    rand_arithmetic Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
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
    auto dist = detail::make_distribution(min, max);
    detail::fill_container(container, count, dist, gen);

    return container;
}

template<
    reversible_container Container,
    boolean Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
) {
    return generate_bool<Container, Generator, Seed>(count, seed);
}

template<
    associative_container Container,
    typename Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
) {
    return generate_uniq<Container, Item, Generator, Seed>(count, min, max, seed);
}

} // rand
#endif  // GENERATE_HPP_
