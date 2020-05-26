#pragma once
#include <ratio>
#include <random>
#include "otmfwd.hpp"

namespace otm
{
	template <std::floating_point T>
	constexpr auto kPiV = static_cast<T>(PiRatio::num) / PiRatio::den;
	constexpr auto kPi = kPiV<Float>;
	
	template <class T>
	constexpr auto kSmallNumV = static_cast<T>(1e-6);
	constexpr auto kSmallNum = kSmallNumV<Float>;

	inline thread_local std::default_random_engine random_engine{std::random_device{}()};

	template <class T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	// [min, max] for integral
	// [min, max) for floating point
	template <class T1 = Float, class T2 = Float, class T = std::common_type_t<T1, T2>>
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

	template <class Ratio, std::floating_point T>
	T Cos(Angle<Ratio, T> t) noexcept
	{
		return std::cos(Angle<RadR, T>{t}.Get());
	}
	
	template <class Ratio, std::floating_point T>
	T Sin(Angle<Ratio, T> t) noexcept
	{
		return std::sin(Angle<RadR, T>{t}.Get());
	}
	
	template <class Ratio, std::floating_point T>
	T Tan(Angle<Ratio, T> t) noexcept
	{
		return std::tan(Angle<RadR, T>{t}.Get());
	}
	
	template <class T>
	Angle<RadR, CommonFloat<T>> Acos(T x) noexcept
	{
		return Angle<RadR, CommonFloat<T>>{std::acos(x)};
	}
	
	template <class T>
	Angle<RadR, CommonFloat<T>> Asin(T y) noexcept
	{
		return Angle<RadR, CommonFloat<T>>{std::asin(y)};
	}
	
	template <class T, class U>
	Angle<RadR, CommonFloat<T, U>> Atan2(T y, U x) noexcept
	{
		return Angle<RadR, CommonFloat<T, U>>{
			std::atan2(ToFloat<U>(y), ToFloat<T>(x))
		};
	}

	template <Arithmetic T, Arithmetic U, Arithmetic V = std::common_type_t<T, U>>
	[[nodiscard]] constexpr bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumV<V>) noexcept
	{
		return Abs(a - b) < tolerance;
	}

	template <class T, size_t L, class V = T>
	[[nodiscard]] constexpr bool IsNearlyEqual(const Vector<T, L>& a, const Vector<T, L>& b, V tolerance = kSmallNumV<V>) noexcept
	{
		for (size_t i=0; i<L; ++i)
			if (!IsNearlyEqual(a[i], b[i], tolerance))
				return false;
		return true;
	}

	template <class T, size_t R, size_t C, class V = T>
	[[nodiscard]] constexpr bool IsNearlyEqual(const Matrix<T, R, C>& a, const Matrix<T, R, C>& b, V tolerance = kSmallNumV<V>) noexcept
	{
		for (size_t i=0; i<R; ++i)
			if (!IsNearlyEqual(a[i], b[i], tolerance))
				return false;
		return true;
	}

	template <std::floating_point T, class V = T>
	[[nodiscard]] constexpr bool IsNearlyEqual(const Quaternion<T>& a, const Quaternion<T>& b, V tolerance = kSmallNumV<V>) noexcept
	{
		return IsNearlyEqual(a.v, b.v, tolerance) && IsNearlyEqual(a.s, b.s, tolerance);
	}

	template <class T, class U = T>
	[[nodiscard]] constexpr bool IsNearlyZero(T a, U tolerance = kSmallNumV<U>) noexcept
	{
		return Abs(a) < tolerance;
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
		for (size_t i=0; i<R; ++i)
			if (!IsNearlyZero(a[i], tolerance))
				return false;
		return true;
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
	
	template <class T, class U = Float>
	CommonFloat<T, U> GetRangePct(const Vector<U, 2>& range, T val) noexcept
	{
		return GetRangePct(range[0], range[1], val);
	}

	template <class T, class U = Float>
	std::common_type_t<T, U> GetRangeValue(const Vector<U, 2>& range, T pct) noexcept
	{
		return Lerp(range[0], range[1], pct);
	}

	template <class T, class U = Float, class V = Float>
	std::common_type_t<T, U, V> MapRngClamp(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng, T val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	template <class T, class U = Float, class V = Float>
	std::common_type_t<T, U, V> MapRng(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng, T val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}

}
