#pragma once
#include "Angle.hpp"
#include "MathUtil.hpp"
#include "Vector.hpp"

namespace otm
{
	struct Quat
	{
		union
		{
			struct { Vec3 v; float s; };
			struct { float x, y, z, w; };
			Vec4 v4;
		};

		constexpr Quat() noexcept: v{}, s{1} {}
		constexpr Quat(const Vec3& v, float s) noexcept: v{v}, s{s} {}
		constexpr Quat(float x, float y, float z, float w) noexcept: x{x}, y{y}, z{z}, w{w} {}
		constexpr Quat(const Vec4& v4) noexcept: v4{v4} {}
		
		Quat(const UVec3& axis, Radians angle) noexcept
			:Quat{axis.Get() * Sin(angle / 2), Cos(angle / 2)}
		{
		}
	};
}
