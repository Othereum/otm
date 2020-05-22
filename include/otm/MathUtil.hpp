#pragma once
#include "Angle.hpp"

namespace otm
{
	template <class T>
	CommonFloat<T> Cos(Angle<RadR, T> t) noexcept
	{
		return std::cos(ToFloat(t.Get()));
	}
	
	template <class T>
	CommonFloat<T> Sin(Angle<RadR, T> t) noexcept
	{
		return std::sin(ToFloat(t.Get()));
	}
	
	template <class T>
	CommonFloat<T> Tan(Angle<RadR, T> t) noexcept
	{
		return std::tan(ToFloat(t.Get()));
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

	template <class T, class U = float>
	CommonFloat<T, U> GetRangePct(const Vector<U, 2>& range, T val) noexcept
	{
		return GetRangePct(range.x, range.y, val);
	}

	template <class T, class U = float>
	std::common_type_t<T, U> GetRangeValue(const Vector<U, 2>& range, T pct) noexcept
	{
		return Lerp(range.x, range.y, pct);
	}

	template <class T, class U = float, class V = float>
	std::common_type_t<T, U, V> MapRngClamp(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng, T val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	template <class T, class U = float, class V = float>
	std::common_type_t<T, U, V> MapRng(const Vector<U, 2>& in_rng, const Vector<V, 2>& out_rng, T val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}
}
