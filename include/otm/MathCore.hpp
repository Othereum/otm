#pragma once
#include <ratio>
#include <random>
#include "otmfwd.hpp"

namespace otm
{
	template <std::floating_point T>
	constexpr auto kPiV = static_cast<T>(PiRatio::num) / PiRatio::den;
	constexpr auto kPi = kPiV<float>;
	
	template <std::floating_point T>
	constexpr auto kSmallNumV = static_cast<T>(1e-6);
	constexpr auto kSmallNum = kSmallNumV<float>;

	template <class T, class... Ts>
	[[nodiscard]] constexpr CommonFloat<T, Ts...> ToFloat(T x) noexcept
	{
		return static_cast<CommonFloat<T, Ts...>>(x);
	}
	
	template <class T1, class T2>
	[[nodiscard]] constexpr auto Min(T1 a, T2 b) noexcept
	{
		using T = std::common_type_t<T1, T2>;
		return static_cast<T>(a) < static_cast<T>(b) ? a : b;
	}

	template <class T1, class T2, class T3, class... Ts>
	[[nodiscard]] constexpr auto Min(T1 x1, T2 x2, T3 x3, Ts... xs) noexcept
	{
		return Min(Min(x1, x2), x3, xs...);
	}

	template <class T1, class T2>
	[[nodiscard]] constexpr auto Max(T1 a, T2 b) noexcept
	{
		using T = std::common_type_t<T1, T2>;
		return static_cast<T>(a) > static_cast<T>(b) ? a : b;
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
		return x >= 0 ? x : -x;
	}

	template <class T, class U, class V = CommonFloat<T>>
	[[nodiscard]] constexpr bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumV<V>) noexcept
	{
		return Abs(a - b) < tolerance;
	}

	template <class T, class U = CommonFloat<T>>
	[[nodiscard]] constexpr bool IsNearlyZero(T a, U tolerance = kSmallNumV<U>) noexcept
	{
		return Abs(a) < tolerance;
	}

	template <class T, class U, class V>
	[[nodiscard]] constexpr CommonFloat<T, U, V> GetRangePct(T min, U max, V val) noexcept
	{
		return ToFloat<U>(val - min) / (max - min);
	}
	
	template <class T, class U, class V> 
	[[nodiscard]] constexpr std::common_type_t<T, U, V> Lerp(T a, U b, V alpha) noexcept
	{
		return a + alpha * (b - a);
	}
	
	inline thread_local std::default_random_engine random_engine{std::random_device{}()};

	// [min, max] for int
	// [min, max) for float
	template <class T1 = float, class T2 = float, class T = std::common_type_t<T1, T2>>
	[[nodiscard]] T Rand(T1 min = 0, T2 max = std::is_integral_v<T> ? std::numeric_limits<T>::max() : 1) noexcept
	{
		using Distribution = std::conditional_t<std::is_integral_v<T>,
			std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;
		
		return Distribution{min, max}(random_engine);
	}

	template <class T, class U>
	[[nodiscard]] CommonFloat<T, U> Gauss(T mean, U stddev) noexcept
	{
		return std::normal_distribution<CommonFloat<T, U>>{
			ToFloat<U>(mean), ToFloat<T>(stddev)
		}(random_engine);
	}
}
