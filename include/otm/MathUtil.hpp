#pragma once
#include "Angle.hpp"

namespace otm
{
	template <class T>
	CommonFloat<T> Cos(Angle<RadR, T> t) noexcept
	{
		return std::cos(ToFloat(t));
	}
	
	template <class T>
	CommonFloat<T> Sin(Angle<RadR, T> t) noexcept
	{
		return std::sin(ToFloat(t));
	}
	
	template <class T>
	CommonFloat<T> Tan(Angle<RadR, T> t) noexcept
	{
		return std::tan(ToFloat(t));
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

	template <class T, class U>
	CommonFloat<T, U> GetRangePct(const Vector<T, 2>& range, U val) noexcept
	{
		return GetRangePct(range.x, range.y, val);
	}

	template <class T, class U>
	std::common_type_t<T, U> GetRangeValue(const Vector<T, 2>& range, U pct) noexcept
	{
		return Lerp(range.x, range.y, pct);
	}

	template <class T, class U, class V>
	std::common_type_t<T, U, V> MapRngClamp(const Vector<T, 2>& in_rng, const Vector<U, 2>& out_rng, V val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	template <class T, class U, class V>
	std::common_type_t<T, U, V> MapRng(const Vector<T, 2>& in_rng, const Vector<U, 2>& out_rng, V val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}
}
