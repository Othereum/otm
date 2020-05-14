#pragma once
#include <compare>
#include "MathCore.hpp"

namespace otm
{
	template <class Ratio, class T>
	struct Angle
	{
		static constexpr auto ratio = static_cast<T>(Ratio::num) / Ratio::den;

		constexpr Angle() noexcept = default;
		constexpr explicit Angle(T r) noexcept :val{r} {}
		
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

		constexpr Angle& operator*=(T f) noexcept { return *this = *this * f; }
		constexpr Angle operator*(T f) const noexcept { return Angle{val * f}; }
		
		constexpr Angle& operator/=(T f) noexcept { return *this = *this / f; }
		constexpr Angle operator/(T f) const noexcept { return Angle{val / f}; }

		constexpr Angle operator-() const noexcept { return Angle{-val}; }

		template <class S>
		constexpr auto operator<=>(const Angle<S>& r) const noexcept { return *this <=> Angle{r}; }
		constexpr auto operator<=>(const Angle&) const noexcept = default;

		[[nodiscard]] constexpr T Get() const noexcept { return val; }
		explicit constexpr operator T() const noexcept { return val; }

		[[nodiscard]] UnitVec<CommonFloat<T>, 2> ToVector() const noexcept;

		[[nodiscard]] static Angle Rand() noexcept
		{
			return Rad{otm::Rand(-kPi, kPi)};
		}

	private:
		T val = 0;
	};

	constexpr Rad operator""_rad(unsigned long long f) noexcept { return Rad{static_cast<float>(f)}; }
	constexpr Rad operator""_rad(long double f) noexcept { return Rad{static_cast<float>(f)}; }

	constexpr Deg operator""_deg(unsigned long long f) noexcept { return Deg{static_cast<float>(f)}; }
	constexpr Deg operator""_deg(long double f) noexcept { return Deg{static_cast<float>(f)}; }
	
	template <class R, class F, class F2>
	Angle<R, F> operator*(F2 f, const Angle<R, F>& r) noexcept
	{
		return r * f;
	}
}
