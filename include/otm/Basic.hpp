#pragma once
#include <concepts>
#include <random>

#ifndef OTM_DEFAULT_FLOAT
#define OTM_DEFAULT_FLOAT float
#endif

namespace otm
{
using Float = OTM_DEFAULT_FLOAT;
static_assert(std::is_floating_point_v<Float>);

template <class... T>
using CommonFloat = std::conditional_t<std::is_floating_point_v<std::common_type_t<T...>>, std::common_type_t<T...>,
                                       std::enable_if_t<std::is_integral_v<std::common_type_t<T...>>, Float>>;

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <class T, class... Ts>
[[nodiscard]] constexpr auto ToFloat(T x) noexcept
{
    return static_cast<CommonFloat<T, Ts...>>(x);
}

constexpr Float operator""_f(unsigned long long f) noexcept
{
    return static_cast<Float>(f);
}

constexpr Float operator""_f(long double f) noexcept
{
    return static_cast<Float>(f);
}

template <class T>
constexpr auto kPiV = static_cast<T>(3.1415926535897932384626433832795l);
constexpr auto kPi = kPiV<Float>;

template <class T>
constexpr auto kSmallNumV = static_cast<T>(1e-8l);
constexpr auto kSmallNum = kSmallNumV<Float>;

inline thread_local std::default_random_engine random_engine{std::random_device{}()};

/**
 * Check if given integer is convertible to target type without loss.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return `true` if it is safely convertible.
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr bool IsSafelyConvertible(From from) noexcept
{
    if constexpr (sizeof(To) > sizeof(From))
    {
        return true;
    }
    else if constexpr (std::is_signed_v<To> == std::is_signed_v<From>)
    {
        if constexpr (sizeof(To) == sizeof(From))
        {
            return true;
        }
        else
        {
            return from <= static_cast<From>(std::numeric_limits<To>::max()) &&
                   from >= static_cast<From>(std::numeric_limits<To>::lowest());
        }
    }
    else if constexpr (std::is_signed_v<To>)
    {
        return from <= static_cast<From>(std::numeric_limits<To>::max());
    }
    else
    {
        return from >= 0;
    }
}

/**
 * Try to cast given integer to target type.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return Converted value.
 * @throw std::domain_error If it is not safely convertible.
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr To SafeCast(From from)
{
    if (IsSafelyConvertible<To>(from))
        return static_cast<To>(from);

    throw std::domain_error{"Cannot convert without loss"};
}

static_assert(IsSafelyConvertible<char>(10));
static_assert(!IsSafelyConvertible<char>(1000));
static_assert(IsSafelyConvertible<int64_t>(1000));
static_assert(IsSafelyConvertible<int64_t>(1000ull));
static_assert(IsSafelyConvertible<int16_t>(30000));
static_assert(IsSafelyConvertible<int16_t>(-30000));
static_assert(!IsSafelyConvertible<int16_t>(300000));
static_assert(!IsSafelyConvertible<int16_t>(-300000));
static_assert(!IsSafelyConvertible<int16_t>(60000u));
static_assert(IsSafelyConvertible<int16_t>(20000u));
static_assert(!IsSafelyConvertible<uint16_t>(-20));
static_assert(IsSafelyConvertible<uint16_t>(2062));

template <std::integral T1, std::integral T2>
[[nodiscard]] constexpr T1 IntLog(T1 x, T2 base) noexcept
{
    T1 cnt = 0;
    while ((x /= base) > 0)
        ++cnt;
    return cnt;
}

template <std::integral T1, std::integral T2>
[[nodiscard]] constexpr T1 IntLogCeil(T1 x, T2 base) noexcept
{
    T1 cnt = 0;
    auto remain = false;

    for (T1 result{}; (result = x / base) > 0; ++cnt)
    {
        remain = remain || x % base;
        x = result;
    }

    return cnt + remain;
}

/**
 * @brief Returns random float in range [0, 1)
 */
[[nodiscard]] inline Float Rand() noexcept
{
    return std::uniform_real_distribution<Float>{0, 1}(random_engine);
}

/**
 * @brief Returns random int in range [min, max]
 */
template <class T, class U>
[[nodiscard]] auto Rand(T min, U max) noexcept requires std::integral<std::common_type_t<T, U>>
{
    return std::uniform_int_distribution<std::common_type_t<T, U>>{min, max}(random_engine);
}

/**
 * @brief Returns random float in range [min, max)
 */
template <class T, class U>
[[nodiscard]] auto Rand(T min, U max) noexcept requires std::floating_point<std::common_type_t<T, U>>
{
    return std::uniform_real_distribution<std::common_type_t<T, U>>{min, max}(random_engine);
}

template <class T, class U>
[[nodiscard]] auto Gauss(T mean, U stddev) noexcept
{
    return std::normal_distribution<CommonFloat<T, U>>{ToFloat<U>(mean), ToFloat<T>(stddev)}(random_engine);
}

template <class T1, class T2>
[[nodiscard]] constexpr auto Min(T1 a, T2 b) noexcept
{
    return a < b ? a : b;
}

template <class T1, class T2, class T3, class... Ts>
[[nodiscard]] constexpr auto Min(T1 x1, T2 x2, T3 x3, Ts... xs) noexcept
{
    return Min(Min(x1, x2), x3, xs...);
}

template <class T1, class T2>
[[nodiscard]] constexpr auto Max(T1 a, T2 b) noexcept
{
    return a > b ? a : b;
}

template <class T1, class T2, class T3, class... Ts>
[[nodiscard]] constexpr auto Max(T1 x1, T2 x2, T3 x3, Ts... xs) noexcept
{
    return Max(Max(x1, x2), x3, xs...);
}

template <class T, class U, class V>
[[nodiscard]] constexpr auto Clamp(T v, U min, V max) noexcept
{
    return Max(Min(v, max), min);
}

template <class T>
[[nodiscard]] constexpr T Abs(T x) noexcept
{
    return x >= T(0) ? x : -x;
}

template <class T>
[[nodiscard]] constexpr T Sign(T x) noexcept
{
    return x >= T(0) ? T(1) : T(-1);
}

template <class T>
[[nodiscard]] auto Sqrt(T x) noexcept
{
    return std::sqrt(ToFloat(x));
}

template <Arithmetic T, Arithmetic U, class V = std::common_type_t<T, U>>
[[nodiscard]] constexpr bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumV<V>) noexcept
{
    return Abs(a - b) <= tolerance;
}

template <class T, class U = T>
[[nodiscard]] constexpr bool IsNearlyZero(T a, U tolerance = kSmallNumV<U>) noexcept
{
    return Abs(a) <= tolerance;
}

template <class T, class U, class V>
[[nodiscard]] constexpr auto GetRangePct(T min, U max, V val) noexcept
{
    return ToFloat<U>(val - min) / (max - min);
}

template <class T, class U, class V>
[[nodiscard]] constexpr auto Lerp(T a, U b, V alpha) noexcept
{
    return a + alpha * (b - a);
}

template <class T, class U, class V, class W, class X>
[[nodiscard]] constexpr auto MapRngClamp(T in_min, U in_max, V out_min, W out_max, X val) noexcept
{
    return Lerp(out_min, out_max, Clamp(GetRangePct(in_min, in_max, val), 0, 1));
}

template <class T, class U, class V, class W, class X>
[[nodiscard]] constexpr auto MapRng(T in_min, U in_max, V out_min, W out_max, X val) noexcept
{
    return Lerp(out_min, out_max, GetRangePct(in_min, in_max, val));
}
} // namespace otm
