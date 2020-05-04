#pragma once
#include "Matrix.hpp"

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
			return Mat4::Scale(scale) * Mat4::Rotation(rot) * Mat4::Translation(pos);
		}
	};
}
