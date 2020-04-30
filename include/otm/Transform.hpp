#pragma once
#include "Matrix.hpp"
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
			return ScaleMatrix(scale) * RotationMatrix(rot) * TranslationMatrix(pos);
		}

		[[nodiscard]] static constexpr Mat4 TranslationMatrix(const Vec3& pos) noexcept
		{
			auto t = Mat4::Identity();
			t[3] << pos.x << pos.y << pos.z;
			return t;
		}

		[[nodiscard]] static constexpr Mat4 RotationMatrix(const Quat& rot) noexcept
		{
			// TODO: Implement
			return Mat4::Identity();
		}

		[[nodiscard]] static constexpr Mat4 ScaleMatrix(const Vec3& scale) noexcept
		{
			auto s = Mat4::Identity();
			s[0][0] = scale.x;
			s[1][1] = scale.y;
			s[2][2] = scale.z;
			return s;
		}
	};
}
