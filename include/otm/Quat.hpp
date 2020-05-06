#pragma once
#include "MathUtil.hpp"

namespace otm
{
	struct Quat
	{
		union
		{
			struct { Vec3 v; float s; };
			struct { float x, y, z, w; };
			Vec4 v4;
			float data[4];
		};

		constexpr Quat() noexcept: v{}, s{1} {}
		constexpr Quat(const Vec3& v, float s) noexcept: v{v}, s{s} {}
		constexpr Quat(float x, float y, float z, float w) noexcept: x{x}, y{y}, z{z}, w{w} {}
		explicit constexpr Quat(const Vec4& v4) noexcept: v4{v4} {}
		
		Quat(const UVec3& axis, Radians angle) noexcept
			:Quat{axis.Get() * Sin(angle / 2), Cos(angle / 2)}
		{
		}

		constexpr Quat& operator*=(const Quat& q) noexcept { return *this = *this * q; }
		constexpr Quat operator*(const Quat& q) const noexcept
		{
			return {s*q.v + q.s*v + (v^q.v), s*q.s - (v|q.v)};
		}

		constexpr void Invert() noexcept { v.Negate(); }
		constexpr Quat operator~() const noexcept { return {-v, s}; }
	};
	
	template <class T>
	Vec3 detail::VecBase<T, 3>::Rotate(const Quat& q) const noexcept
	{
		return (q * Quat{*this, 0} * ~q).v;
	}
}
