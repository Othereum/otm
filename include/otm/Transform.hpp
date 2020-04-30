#pragma once
#include "MathUtil.hpp"
#include "Quat.hpp"

namespace otm
{
	struct Transform
	{
		Vec3 pos;
		Quat rot;
		Vec3 scale;

		constexpr Transform() noexcept = default;
		explicit constexpr Transform(const Vec3& pos) noexcept: Transform{pos, {}} {}
		explicit constexpr Transform(const Quat& rot) noexcept: Transform{{}, rot} {}

		constexpr Transform(const Vec3& pos, const Quat& rot, const Vec3& scale = Vec3::One()) noexcept:
			pos{pos}, rot{rot}, scale{scale}
		{
		}

		[[nodiscard]] constexpr Mat4 ToMatrix() const noexcept
		{
			return Scale() * Rotation() * Translation();
		}

		[[nodiscard]] constexpr Mat4 Translation() const noexcept
		{
			auto t = Mat4::Identity();
			t[3] << pos.x << pos.y;
			return t;
		}

		// TODO: Implement
		[[nodiscard]] Mat4 Rotation() const noexcept
		{
		}

		[[nodiscard]] constexpr Mat4 Scale() const noexcept
		{
			auto s = Mat4::Identity();
			s[0][0] = scale.x;
			s[1][1] = scale.y;
			return s;
		}
	};
}
