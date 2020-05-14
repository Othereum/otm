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
	constexpr auto kSmallNumberV = static_cast<T>(1e-8);
	constexpr auto kSmallNumber = kSmallNumberV<float>;
	
	template <std::floating_point T>
	constexpr auto kKindaSmallNumberV = static_cast<T>(1e-4);
	constexpr auto kKindaSmallNumber = kKindaSmallNumberV<float>;

	template <class T, class... U>
	[[nodiscard]] constexpr CommonFloat<T, U...> ToFloat(T x) noexcept
	{
		return static_cast<CommonFloat<T, U...>>(x);
	}
	
	template <class T, class U>
	[[nodiscard]] constexpr std::common_type_t<T, U> Min(T a, U b) noexcept
	{
		return a < b ? a : b;
	}

	template <class T, class U>
	[[nodiscard]] constexpr std::common_type_t<T, U> Max(T a, U b) noexcept
	{
		return a > b ? a : b;
	}

	template <class T, class U, class V>
	[[nodiscard]] constexpr std::common_type_t<T, U, V> Clamp(T v, U min, V max) noexcept
	{
		return Max(Min(v, max), min);
	}

	template <class T>
	[[nodiscard]] constexpr T Abs(T x) noexcept
	{
		return x >= 0 ? x : -x;
	}

	template <class T, class U, class V = CommonFloat<T>>
	[[nodiscard]] constexpr bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumberV<V>) noexcept
	{
		return Abs(a - b) < tolerance;
	}

	template <class T, class U = CommonFloat<T>>
	[[nodiscard]] constexpr bool IsNearlyZero(T a, U tolerance = kSmallNumberV<U>) noexcept
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
	template <class T = float>
	[[nodiscard]] T Rand(T min = 0, T max = std::is_integral_v<T> ? std::numeric_limits<T>::max() : 1) noexcept
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
