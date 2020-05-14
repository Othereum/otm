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
			std::atan2(
				static_cast<CommonFloat<T, U>>(y),
				static_cast<CommonFloat<T, U>>(x)
			)
		};
	}

	inline Vec2 RandUnitVec() noexcept
	{
		return R2V(RandAng());
	}

	inline Vec2 RandVec(const Vec2& min, const Vec2& max) noexcept
	{
		return {Rand(min.x, max.x), Rand(min.y, max.y)};
	}

	inline float GetRangePct(const Vec2& range, float val) noexcept
	{
		return GetRangePct(range.x, range.y, val);
	}

	inline float GetRangeValue(const Vec2& range, float pct) noexcept
	{
		return Lerp(range.x, range.y, pct);
	}

	inline float MapRngClamp(const Vec2& in_rng, const Vec2& out_rng, float val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	inline float MapRng(const Vec2& in_rng, const Vec2& out_rng, float val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}
}
