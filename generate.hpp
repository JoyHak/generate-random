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

//region items concepts
using std::integral, std::floating_point;

template<typename T>
concept arithmetic = integral<T> || floating_point<T>;

template<typename Gen>
concept uniform_generator = std::uniform_random_bit_generator<Gen>;

/**
 * According to the <a href="https://eel.is/c++draft/rand#req.genl-1">standart</a>:
 * <br><i>where the template parameters are not constrained, the names of template parameters
 * are used to express type requirements on an instantiated template T</i>.<br><br>
 *
 * The standard does not describe <i>concepts</i> that would allow efficient type deduction for wrappers.
 * So we will have to create our own <i>concepts</i> based on <a href="https://eel.is/c++draft/rand#req.genl-1">definition rules</a>
 * for template paramer <i>IntType</i>.
 * Concepts will help the compiler choose template specifications that will allow to generate
 * container of any type: <code>char, bool, string</code>, ...
 */

template<typename T>
concept rand_integer =
    std::same_as<T, short>
 || std::same_as<T, int>
 || std::same_as<T, long>
 || std::same_as<T, long long>
 || std::same_as<T, unsigned short>
 || std::same_as<T, unsigned int>
 || std::same_as<T, unsigned long>
 || std::same_as<T, unsigned long long>
#if _LIBCPP_HAS_INT128
 || std::same_as<T, __int128_t>   // extension
 || std::same_as<T, __uint128_t>  // extension
#endif
 || std::same_as<T, int8_t>       // extension
 || std::same_as<T, uint8_t>;     // extension

template<typename T>
concept rand_floating =
    std::same_as<T, float>
 || std::same_as<T, double>
 || std::same_as<T, long double>;

template<typename T>
concept rand_arithmetic = rand_integer<T> || rand_floating<T>;

/**
 * For other types that uniform distribution does not support we use custom concepts
 */

template<typename T>
concept boolean = std::same_as<T, bool>;

template<typename T>
concept character = 
    std::same_as<T, char>
 || std::same_as<T, signed char>
 || std::same_as<T, unsigned char>
 || std::same_as<T, wchar_t>
 || std::same_as<T, char8_t>
 || std::same_as<T, char16_t>
 || std::same_as<T, char32_t>;
//endregion

//region containers concepts
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
//endregion

namespace detail {

//region fillers
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
//endregion

//region distributions
template<integral T>
class uniq_int_distribution {
 private:
    T min_val;
    T max_val;
    T count;
    T max_count;
    T A;  // LCG multiplier: gcd(A, max_count) == 1
    T B;  // shift
    bool seed_initialized;

    T gcd(T a, T b) const {
        while (b != 0) {
            T temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    T find_coprime(T N) const {
        if (N <= 1) return 1;
        for (T a = 2; a < N; ++a) {
            if (gcd(a, N) == 1) {
                return a;
            }
        }
        return 1;
    }

 public:
    uniq_int_distribution(T min, T max)
        : min_val(min), max_val(max),
          count(0),
          max_count(max - min + 1),
          A(1), B(0),
          seed_initialized(false) {}

    template<uniform_generator Generator>
    T operator()(Generator& gen) {
        if (max_count == 1) {
            return min_val;
        }

        if (!seed_initialized) {
            std::uniform_int_distribution<T> dist(
                0, std::numeric_limits<T>::max()
            );

            B = dist(gen);
            A = find_coprime(max_count);

            if (A == 1 && max_count > 2) {
                A = 2; // fallback

                while (gcd(A, max_count) != 1 && A < max_count) {
                    ++A;
                }
            }
            seed_initialized = true;
        }

        // Generate the unique index: f(i) = (A * i + B) mod N
        T index = (A * count + B) % max_count;
        count++;
        return min_val + index;
    }
};

template<typename T>
using limits = std::numeric_limits<T>;

template<integral Item>
inline auto make_uniq_distribution(Item min = 0, Item max = limits<Item>::max()) {
    return uniq_int_distribution<Item>(min, max);
}

template<floating_point Item>
inline auto make_uniq_distribution(Item min = 0, Item max = limits<Item>::max()) {
    return std::uniform_real_distribution<Item>(min, max);
}

template<boolean Item>
inline auto make_distribution(Item min = false, Item max = true) {
    return std::uniform_int_distribution<short>((short)min, (short)max);
}

template<character Item>
inline auto make_distribution(Item min = 0, Item max = limits<Item>::max()) {
    return std::uniform_int_distribution<short>((short)min, (short)max);
}

template<rand_integer Item>
inline auto make_distribution(Item min = 0, Item max = limits<Item>::max()) {
    return std::uniform_int_distribution<Item>(min, max);
}

template<rand_floating Item>
inline auto make_distribution(Item min = 0, Item max = limits<Item>::max()) {
    return std::uniform_real_distribution<Item>(min, max);
}

inline auto make_distribution() {
    // 50% true, 50% false
    return std::bernoulli_distribution(0.5);
}
//endregion

//region helpers

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

//endregion

} // namespace detail

//region generators
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
    arithmetic Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate_uniq(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
) {
    // generate_bool() performs faster for [0, 1] range
    if constexpr (boolean<Item> || boolean<typename Container::value_type>) {
        return generate_bool<Container, Generator, Seed>(count, seed);
    }
    if constexpr (integral<Item>) {
        if (min == 0 && max <= 1) {
            return generate_bool<Container, Generator, Seed>(count, seed);
        }
    }

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
    arithmetic Item = typename Container::value_type,
    uniform_generator Generator = std::mt19937,
    typename Seed = unsigned int
>
Container generate(
    size_t count = 10,
    Item min = 1, Item max = 26,
    Seed seed = std::random_device{}()
) {
    // uniform distribution below is inefficient for [0, 1] range
    if constexpr (boolean<Item> || boolean<typename Container::value_type>) {
        return generate_bool<Container, Generator, Seed>(count, seed);
    }
    if constexpr (integral<Item>) {
        if (min == 0 && max <= 1) {
            return generate_bool<Container, Generator, Seed>(count, seed);
        }
    }

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
    associative_container Container,
    arithmetic Item = typename Container::value_type,
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
//endregion generators

} // namespace rand
#endif  // GENERATE_HPP_
