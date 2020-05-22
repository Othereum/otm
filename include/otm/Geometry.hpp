#pragma once
#include "Quat.hpp"
#include "Matrix.hpp"

namespace otm
{
	template <class T>
	constexpr Matrix<CommonFloat<T>, 3, 3> MakeSimpleViewProj(const Vector<T, 2>& screen) noexcept
	{
		Matrix<CommonFloat<T>, 3, 3> proj;
		proj[0][0] = 2 / ToFloat(screen[0]);
		proj[1][1] = 2 / ToFloat(screen[1]);
		proj[2][2] = 1;
		return proj;
	}

	template <std::floating_point T>
	constexpr Matrix<T, 3, 3> MakeRotation(const Quaternion<T>& rotation) noexcept
	{
		const auto& [x, y, z] = rotation.v.data;
		const auto& w = rotation.s;
		
		return {
			1 - 2*(y*y + z*z),	2*(x*y + w*z),		2*(x*z - w*y),
			2*(x*y - w*z),		1 - 2*(x*x + z*z),	2*(y*z + w*x),
			2*(x*z + w*y),		2*(y*z - w*x),		1 - 2*(x*x + y*y)
		};
	}

	template <class T>
	constexpr Matrix<T, 3, 3> MakeScale(const Vector<T, 3>& scale) noexcept
	{
		Matrix<T, 3, 3> s;
		s[0][0] = scale[0];
		s[1][1] = scale[1];
		s[2][2] = scale[2];
		return s;
	}
	
	template <class T>
	constexpr Matrix<T, 4, 4> MakeTranslation(const Vector<T, 3>& pos) noexcept
	{
		return Matrix<T, 4, 4>::Identity(pos.ToRowMatrix(), {0, 3});
	}

	template <std::floating_point T>
	static Matrix<T, 4, 4> MakeLookAt(const Vector<T, 3>& eye, const Vector<T, 3>& target, const Vector<T, 3>& up)
	{
		auto k = target - eye; k.Normalize();
		auto i = up ^ k; i.Normalize();
		auto j = k ^ i; j.Normalize();
		Vec3 t{eye|i, eye|j, eye|k}; t.Negate();

		return {
			i[0], j[0], k[0], 0,
			i[1], j[1], k[1], 0,
			i[2], j[2], k[2], 0,
			t[0], t[1], t[2], 1
		};
	}
}
