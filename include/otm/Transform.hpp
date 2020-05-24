#pragma once
#include "Geometry.hpp"

namespace otm
{
	struct Transform
	{
		Vec3 pos;
		Quat rot;
		Vec3 scale = Vec3::One();

		constexpr Transform() noexcept = default;
		explicit constexpr Transform(const Vec3& pos) noexcept: Transform{pos, {}} {}
		explicit constexpr Transform(const Quat& rot) noexcept: Transform{{}, rot} {}

		constexpr Transform(const Vec3& pos, const Quat& rot, const Vec3& scale = Vec3::One()) noexcept
			:pos{pos}, rot{rot}, scale{scale}
		{
		}

		explicit Transform(const Mat4& m) noexcept
		{
			pos = Vec3{m[3]};
			scale = {m[0].Len(), m[1].Len(), m[2].Len()};
			// TODO: Extract rotation
		}

		[[nodiscard]] constexpr Mat4 ToMatrix() const noexcept
		{
			return Mat4::Identity(MakeScale(scale)) * Mat4::Identity(MakeRotation(rot)) * MakeTranslation(pos);
		}
	};
}
