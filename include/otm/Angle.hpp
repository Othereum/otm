#pragma once
#include <compare>
#include "MathCore.hpp"

namespace otm
{
	template <class R, class F = float>
	struct Angle
	{
		static constexpr auto ratio = static_cast<F>(R::num) / R::den;

		constexpr Angle() noexcept = default;
		constexpr explicit Angle(F r) noexcept :val{r} {}
		
		template <class S>
		constexpr Angle(const Angle<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr Angle& operator=(const Angle<S>& r) noexcept { val = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr Angle& operator+=(const Angle<S>& r) noexcept { return *this = *this + r; }

		template <class S>
		constexpr Angle operator+(const Angle<S>& r) const noexcept { return *this + Angle{r}; }
		constexpr Angle operator+(const Angle& r) const noexcept { return Angle{val + r.Get()}; }

		template <class S>
		constexpr Angle& operator-=(const Angle<S>& r) noexcept { return *this = *this - r; }

		template <class S>
		constexpr Angle operator-(const Angle<S>& r) const noexcept { return *this - Angle{r}; }
		constexpr Angle operator-(const Angle& r) const noexcept { return Angle{val - r.Get()}; }

		constexpr Angle& operator*=(F f) noexcept { return *this = *this * f; }
		constexpr Angle operator*(F f) const noexcept { return Angle{val * f}; }
		
		constexpr Angle& operator/=(F f) noexcept { return *this = *this / f; }
		constexpr Angle operator/(F f) const noexcept { return Angle{val / f}; }

		constexpr Angle operator-() const noexcept { return Angle{-val}; }

		template <class S>
		constexpr auto operator<=>(const Angle<S>& r) const noexcept { return *this <=> Angle{r}; }
		constexpr auto operator<=>(const Angle&) const noexcept = default;

		[[nodiscard]] constexpr F Get() const noexcept { return val; }
		explicit constexpr operator F() const noexcept { return val; }

	private:
		F val = 0;
	};

	using RadianRatio = std::ratio<PiRatio::num, PiRatio::den * 180>;
	using DegreeRatio = std::ratio<1>;

	using Radians = Angle<RadianRatio>;
	using Degrees = Angle<DegreeRatio>;

	constexpr Radians operator""_rad(unsigned long long f) noexcept { return Radians{static_cast<float>(f)}; }
	constexpr Radians operator""_rad(long double f) noexcept { return Radians{static_cast<float>(f)}; }

	constexpr Degrees operator""_deg(unsigned long long f) noexcept { return Degrees{static_cast<float>(f)}; }
	constexpr Degrees operator""_deg(long double f) noexcept { return Degrees{static_cast<float>(f)}; }
	
	template <class R, class F, class F2>
	Angle<R, F> operator*(F2 f, const Angle<R, F>& r) noexcept
	{
		return r * f;
	}
}
