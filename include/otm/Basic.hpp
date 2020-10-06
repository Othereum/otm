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
using CommonFloat = std::common_type_t<Float, T...>;

constexpr Float operator""_f(unsigned long long f) noexcept
{
    return static_cast<Float>(f);
}

constexpr Float operator""_f(long double f) noexcept
{
    return static_cast<Float>(f);
}

constexpr auto kPi = 3.1415926535897932384626433832795_f;
constexpr auto kSmallNum = 1e-8_f;

inline thread_local std::default_random_engine random_engine{std::random_device{}()};

/**
 * Check if given integer is convertible to target type without loss.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return `true` if it is safely convertible.
 */
template <class To, class From>
[[nodiscard]] constexpr std::enable_if_t<std::is_integral_v<To> && std::is_integral_v<From>, bool> IsSafelyConvertible(
    From from) noexcept
{
    if constexpr (sizeof(To) > sizeof(From))
        return true;

    else if constexpr (std::is_signed_v<To> == std::is_signed_v<From>)
    {
        if constexpr (sizeof(To) == sizeof(From))
            return true;

        else
            return from <= static_cast<From>(std::numeric_limits<To>::max()) &&
                   from >= static_cast<From>(std::numeric_limits<To>::lowest());
    }

    else if constexpr (std::is_signed_v<To>)
        return from <= static_cast<From>(std::numeric_limits<To>::max());

    else
        return from >= 0;
}

/**
 * Try to cast given integer to target type.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return Converted value.
 * @throw std::domain_error If it is not safely convertible.
 */
template <class To, class From>
[[nodiscard]] constexpr std::enable_if_t<std::is_integral_v<To> && std::is_integral_v<From>, To> SafeCast(From from)
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

template <class T1, class T2>[[nodiscard]] constexpr T1 IntLog(T1 x, T2 base) noexcept
{
    static_assert(std::is_integral_v<T1> && std::is_integral_v<T2>);

    T1 cnt = 0;
    while ((x /= base) > 0)
        ++cnt;
    return cnt;
}

template <class T1, class T2>[[nodiscard]] constexpr T1 IntLogCeil(T1 x, T2 base) noexcept
{
    static_assert(std::is_integral_v<T1> && std::is_integral_v<T2>);

    T1 cnt = 0;
    auto remain = false;

    for (T1 result{}; (result = x / base) > 0; ++cnt)
    {
        remain = remain || x % base;
        x = result;
    }

    return cnt + remain;
}

template <class T>[[nodiscard]] constexpr T PadToPowerOf2(T x) noexcept
{
    return 1 << LogCeil(x, 2);
}

// [min, max] for integral
// [min, max) for floating point
template <class T1 = Float, class T2 = T1, class T = std::common_type_t<T1, T2>>
[[nodiscard]] T Rand(T1 min = 0, T2 max = std::is_integral_v<T1> ? std::numeric_limits<T1>::max() : T1(1)) noexcept
{
    using Distribution =
        std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

    return Distribution{T(min), T(max)}(random_engine);
}

template <class T, class U>[[nodiscard]] CommonFloat<T, U> Gauss(T mean, U stddev) noexcept
{
    return std::normal_distribution<CommonFloat<T, U>>{ToFloat<U>(mean), ToFloat<T>(stddev)}(random_engine);
}

template <class T, class... Ts>[[nodiscard]] constexpr CommonFloat<T, Ts...> ToFloat(T x) noexcept
{
    return static_cast<CommonFloat<T, Ts...>>(x);
}

template <class T1, class T2>[[nodiscard]] constexpr auto Min(T1 a, T2 b) noexcept
{
    return a < b ? a : b;
}

template <class T1, class T2, class T3, class... Ts>
[[nodiscard]] constexpr auto Min(T1 x1, T2 x2, T3 x3, Ts... xs) noexcept
{
    return Min(Min(x1, x2), x3, xs...);
}

template <class T, size_t L>[[nodiscard]] constexpr T Min(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Min(m, v[i]);
    return m;
}

template <class T1, class T2>[[nodiscard]] constexpr auto Max(T1 a, T2 b) noexcept
{
    return a > b ? a : b;
}

template <class T1, class T2, class T3, class... Ts>
[[nodiscard]] constexpr auto Max(T1 x1, T2 x2, T3 x3, Ts... xs) noexcept
{
    return Max(Max(x1, x2), x3, xs...);
}

template <class T, size_t L>[[nodiscard]] constexpr T Max(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Max(m, v[i]);
    return m;
}

template <class T, class U, class V>[[nodiscard]] constexpr auto Clamp(T v, U min, V max) noexcept
{
    return Max(Min(v, max), min);
}

template <class T>[[nodiscard]] constexpr T Abs(T x) noexcept
{
    return x >= T(0) ? x : -x;
}

template <class T>[[nodiscard]] constexpr T Sign(T x) noexcept
{
    return x >= T(0) ? T(1) : T(-1);
}

template <class Ratio, class T> T Cos(Angle<Ratio, T> t) noexcept
{
    return std::cos(Angle<RadR, T>{t}.Get());
}

template <class Ratio, class T> T Sin(Angle<Ratio, T> t) noexcept
{
    return std::sin(Angle<RadR, T>{t}.Get());
}

template <class Ratio, class T> T Tan(Angle<Ratio, T> t) noexcept
{
    return std::tan(Angle<RadR, T>{t}.Get());
}

template <class T> Angle<RadR, CommonFloat<T>> Acos(T x) noexcept
{
    return Angle<RadR, CommonFloat<T>>{std::acos(ToFloat(x))};
}

template <class T> Angle<RadR, CommonFloat<T>> Asin(T y) noexcept
{
    return Angle<RadR, CommonFloat<T>>{std::asin(ToFloat(y))};
}

template <class T> Angle<RadR, CommonFloat<T>> Atan(T x) noexcept
{
    return Angle<RadR, CommonFloat<T>>{std::atan(ToFloat(x))};
}

template <class T, class U> Angle<RadR, CommonFloat<T, U>> Atan2(T y, U x) noexcept
{
    return Angle<RadR, CommonFloat<T, U>>{std::atan2(ToFloat<U>(y), ToFloat<T>(x))};
}

template <class T, class U, class V = std::common_type_t<T, U>, std::enable_if_t<std::is_arithmetic_v<V>, int> = 0>
[[nodiscard]] constexpr bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumV<V>) noexcept
{
    return Abs(a - b) <= tolerance;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Vector<T, L>& a, const Vector<T, L>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < L; ++i)
        if (!IsNearlyEqual(a[i], b[i], tolerance))
            return false;
    return true;
}

template <class T, size_t R, size_t C, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Matrix<T, R, C>& a, const Matrix<T, R, C>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < R; ++i)
        if (!IsNearlyEqual(a[i], b[i], tolerance))
            return false;
    return true;
}

template <class T, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Quaternion<T>& a, const Quaternion<T>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    return IsNearlyEqual(a.v, b.v, tolerance) && IsNearlyEqual(a.s, b.s, tolerance);
}

template <class T, class U = T>[[nodiscard]] constexpr bool IsNearlyZero(T a, U tolerance = kSmallNumV<U>) noexcept
{
    return Abs(a) <= tolerance;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyZero(const Vector<T, L>& a, V tolerance = kSmallNumV<V>) noexcept
{
    for (auto x : a)
        if (!IsNearlyZero(x, tolerance))
            return false;
    return true;
}

template <class T, size_t R, size_t C, class V = T>
[[nodiscard]] constexpr bool IsNearlyZero(const Matrix<T, R, C>& a, V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < R; ++i)
        if (!IsNearlyZero(a[i], tolerance))
            return false;
    return true;
}

template <class T, class U, class V>
[[nodiscard]] constexpr CommonFloat<T, U, V> GetRangePct(T min, U max, V val) noexcept
{
    return ToFloat<U>(val - min) / (max - min);
}

template <class T, class U, class V>[[nodiscard]] constexpr std::common_type_t<T, U, V> Lerp(T a, U b, V alpha) noexcept
{
    return a + alpha * (b - a);
}

template <size_t L, class T, class U, class V>
[[nodiscard]] constexpr auto Lerp(const Vector<T, L>& a, const Vector<U, L>& b, V alpha) noexcept
{
    return a + alpha * (b - a);
}

template <class T, class U = Float>
[[nodiscard]] constexpr CommonFloat<T, U> GetRangePct(const Vector<U, 2>& range, T val) noexcept
{
    return GetRangePct(range[0], range[1], val);
}

template <class T, class U = Float>
[[nodiscard]] constexpr std::common_type_t<T, U> GetRangeValue(const Vector<U, 2>& range, T pct) noexcept
{
    return Lerp(range[0], range[1], pct);
}

template <class T, class U = Float, class V = Float>
[[nodiscard]] constexpr std::common_type_t<T, U, V> MapRngClamp(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng,
                                                                T val) noexcept
{
    const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
    return GetRangeValue(out_rng, pct);
}

template <class T, class U = Float, class V = Float>
[[nodiscard]] constexpr std::common_type_t<T, U, V> MapRng(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng,
                                                           T val) noexcept
{
    return GetRangeValue(out_rng, GetRangePct(in_rng, val));
}
} // namespace otm
