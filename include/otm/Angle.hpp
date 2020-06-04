#pragma once
#include "Basic.hpp"

namespace otm
{
	template <class Ratio, class T>
	struct Angle
	{
		static_assert(std::is_floating_point_v<T>);
		
		static constexpr auto ratio = static_cast<T>(Ratio::num) / static_cast<T>(Ratio::den);

		constexpr Angle() noexcept = default;
		constexpr explicit Angle(T r) noexcept :val{r} {}
		
		template <class S>
		constexpr Angle(const Angle<S, T>& r) noexcept
			:val{ r.Get() / r.ratio * ratio }
		{
		}

		template <class S, class T2>
		explicit constexpr Angle(const Angle<S, T2>& r) noexcept
			:val{ static_cast<T>(r.Get() / r.ratio) * ratio }
		{
		}

		template <class S>
		constexpr Angle& operator=(const Angle<S, T>& r) noexcept { val = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr Angle& operator+=(const Angle<S, T>& r) noexcept { return *this = *this + r; }

		template <class S>
		constexpr Angle operator+(const Angle<S, T>& r) const noexcept { return *this + Angle{r}; }
		constexpr Angle operator+(const Angle& r) const noexcept { return Angle{val + r.Get()}; }

		template <class S>
		constexpr Angle& operator-=(const Angle<S, T>& r) noexcept { return *this = *this - r; }

		template <class S>
		constexpr Angle operator-(const Angle<S, T>& r) const noexcept { return *this - Angle{r}; }
		constexpr Angle operator-(const Angle& r) const noexcept { return Angle{val - r.Get()}; }

		constexpr Angle& operator*=(T f) noexcept { return *this = *this * f; }
		constexpr Angle operator*(T f) const noexcept { return Angle{val * f}; }
		
		constexpr Angle& operator/=(T f) noexcept { return *this = *this / f; }
		constexpr Angle operator/(T f) const noexcept { return Angle{val / f}; }

		constexpr Angle operator-() const noexcept { return Angle{-val}; }
		
		constexpr bool operator<(const Angle& r) const noexcept { return val < r.val; }
		constexpr bool operator>(const Angle& r) const noexcept { return val > r.val; }
		constexpr bool operator<=(const Angle& r) const noexcept { return val <= r.val; }
		constexpr bool operator>=(const Angle& r) const noexcept { return val >= r.val; }
		
		template <class R2, class T2>
		constexpr bool operator<(const Angle<R2, T2>& r) const noexcept { return *this < Angle{r}; }
		
		template <class R2, class T2>
		constexpr bool operator>(const Angle<R2, T2>& r) const noexcept { return *this > Angle{r}; }
		
		template <class R2, class T2>
		constexpr bool operator<=(const Angle<R2, T2>& r) const noexcept { return *this <= Angle{r}; }
		
		template <class R2, class T2>
		constexpr bool operator>=(const Angle<R2, T2>& r) const noexcept { return *this >= Angle{r}; }

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

	constexpr Rad operator""_rad(unsigned long long f) noexcept { return Rad{static_cast<Float>(f)}; }
	constexpr Rad operator""_rad(long double f) noexcept { return Rad{static_cast<Float>(f)}; }

	constexpr Deg operator""_deg(unsigned long long f) noexcept { return Deg{static_cast<Float>(f)}; }
	constexpr Deg operator""_deg(long double f) noexcept { return Deg{static_cast<Float>(f)}; }
	
	template <class R, class F, class F2>
	Angle<R, F> operator*(F2 f, const Angle<R, F>& r) noexcept
	{
		return r * f;
	}
}
