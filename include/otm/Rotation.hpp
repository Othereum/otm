#pragma once
#include <compare>
#include "MathCore.hpp"

namespace otm
{
	template <class R>
	struct Angle
	{
		static constexpr auto ratio = static_cast<float>(R::num) / R::den;

		constexpr Angle() noexcept = default;
		constexpr explicit Angle(float r) noexcept :val{r} {}
		
		template <class S>
		constexpr Angle(const Angle<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr Angle& operator=(const Angle<S>& r) noexcept { val = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr Angle& operator+=(const Angle<S>& r) noexcept { return *this = *this + r; }

		template <class S>
		constexpr Angle operator+(const Angle<S>& r) const noexcept { return *this + Angle{r}; }
		constexpr Angle operator+(const Angle& r) const noexcept { return Angle{val + r.Get()}; }

		constexpr Angle operator*(const float f) const noexcept { return Angle{val * f}; }
		constexpr Angle operator/(const float f) const noexcept { return Angle{val / f}; }

		constexpr Angle operator-() const noexcept { return Angle{-val}; }

		template <class S>
		constexpr auto operator<=>(const Angle<S>& r) const noexcept { return *this <=> Angle{r}; }
		constexpr auto operator<=>(const Angle& r) const noexcept = default;

		[[nodiscard]] constexpr float Get() const noexcept { return val; }

	private:
		float val = 0.f;
	};

	using Radians = Angle<std::ratio<PiRatio::num, PiRatio::den * 180>>;
	using Degrees = Angle<std::ratio<1>>;

	constexpr Radians operator""_rad(unsigned long long f) noexcept { return Radians{static_cast<float>(f)}; }
	constexpr Radians operator""_rad(long double f) noexcept { return Radians{static_cast<float>(f)}; }

	constexpr Degrees operator""_deg(unsigned long long f) noexcept { return Degrees{static_cast<float>(f)}; }
	constexpr Degrees operator""_deg(long double f) noexcept { return Degrees{static_cast<float>(f)}; }
	
	template <class R>
	Angle<R> operator*(float f, const Angle<R>& r) noexcept
	{
		return r * f;
	}
}
