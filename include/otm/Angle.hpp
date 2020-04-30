#pragma once
#include <compare>
#include "MathCore.hpp"

namespace otm
{
	template <class R>
	struct Rotation
	{
		static constexpr auto ratio = static_cast<float>(R::num) / R::den;

		constexpr Rotation() noexcept = default;
		constexpr explicit Rotation(float r) noexcept :val{r} {}
		
		template <class S>
		constexpr Rotation(const Rotation<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr Rotation& operator=(const Rotation<S>& r) noexcept { val = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr Rotation& operator+=(const Rotation<S>& r) noexcept { return *this = *this + r; }

		template <class S>
		constexpr Rotation operator+(const Rotation<S>& r) const noexcept { return *this + Rotation{r}; }
		constexpr Rotation operator+(const Rotation& r) const noexcept { return Rotation{val + r.Get()}; }

		constexpr Rotation operator*(const float f) const noexcept { return Rotation{val * f}; }
		constexpr Rotation operator/(const float f) const noexcept { return Rotation{val / f}; }

		constexpr Rotation operator-() const noexcept { return Rotation{-val}; }

		template <class S>
		constexpr auto operator<=>(const Rotation<S>& r) const noexcept { return *this <=> Rotation{r}; }
		constexpr auto operator<=>(const Rotation& r) const noexcept = default;

		[[nodiscard]] constexpr float Get() const noexcept { return val; }

	private:
		float val = 0.f;
	};

	using Radians = Rotation<std::ratio<PiRatio::num, PiRatio::den * 180>>;
	using Degrees = Rotation<std::ratio<1>>;

	constexpr Radians operator""_rad(unsigned long long f) noexcept { return Radians{static_cast<float>(f)}; }
	constexpr Radians operator""_rad(long double f) noexcept { return Radians{static_cast<float>(f)}; }

	constexpr Degrees operator""_deg(unsigned long long f) noexcept { return Degrees{static_cast<float>(f)}; }
	constexpr Degrees operator""_deg(long double f) noexcept { return Degrees{static_cast<float>(f)}; }
	
	template <class R>
	Rotation<R> operator*(float f, const Rotation<R>& r) noexcept
	{
		return r * f;
	}
}
