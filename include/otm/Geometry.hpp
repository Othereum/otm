#pragma once
#include "Quat.hpp"
#include "Matrix.hpp"

namespace otm
{
	template <size_t L = 3, std::floating_point T = Float>
	constexpr Matrix<T, L> MakeRotation(const Quaternion<T>& rotation) noexcept
	{
		static_assert(L >= 3);
		
		const auto& [x, y, z] = rotation.v.data;
		const auto& w = rotation.s;

		Matrix<T, L> m;
		m[0] << 1 - 2*(y*y + z*z) << 2*(x*y + w*z) << 2*(x*z - w*y);
		m[1] << 2*(x*y - w*z) << 1 - 2*(x*x + z*z) << 2*(y*z + w*x);
		m[2] << 2*(x*z + w*y) << 2*(y*z - w*x) << 1 - 2*(x*x + y*y);

		for (size_t i=3; i<L; ++i)
			m[i][i] = 1;
		
		return m;
	}

	template <size_t L = 3, class T = Float>
	constexpr Matrix<T, L> MakeScale(const Vector<T, 3>& scale) noexcept
	{
		static_assert(L >= 3);
		
		Matrix<T, L> s;
		s[0][0] = scale[0];
		s[1][1] = scale[1];
		s[2][2] = scale[2];
		
		for (size_t i=3; i<L; ++i)
			s[i][i] = 1;
		
		return s;
	}
	
	template <class T = Float>
	constexpr Matrix<T, 4> MakeTranslation(const Vector<T, 3>& pos) noexcept
	{
		return Matrix<T, 4>::Identity(pos.ToRowMatrix(), {0, 3});
	}

	template <size_t L = 2, class T = Float>
	constexpr Matrix<CommonFloat<T>, L> MakeSimpleViewProj(const Vector<T, 2>& screen) noexcept
	{
		static_assert(L >= 2);
		
		Matrix<CommonFloat<T>, L> proj;
		proj[0][0] = 2 / ToFloat(screen[0]);
		proj[1][1] = 2 / ToFloat(screen[1]);
		
		for (size_t i=2; i<L; ++i)
			proj[i][i] = 1;
		
		return proj;
	}

	template <std::floating_point T = Float>
	constexpr Matrix<T, 4> MakeOrtho(const Vector<T, 2>& screen, T near, T far) noexcept
	{
		auto m = MakeSimpleViewProj<4>(screen);
		m[2][2] = 1/(far-near);
		m[3][2] = near/(near-far);
		return m;
	}

	template <std::floating_point T = Float>
	constexpr Matrix<T, 4> MakePerspective(const Vector<T, 2>& screen, T near, T far, Angle<RadR, T> hfov) noexcept
	{
		const auto y_scale = 1/Tan(hfov/2);
		const auto x_scale = y_scale * (screen.y / screen.x);

		return {
			x_scale, 0, 0, 0,
			0, y_scale, 0, 0,
			0, 0, far/(far-near), 1,
			0, 0, (-near*far)/(far-near), 0
		};
	}

	template <std::floating_point T = Float>
	static Matrix<T, 4> MakeLookAt(const Vector<T, 3>& eye, const Vector<T, 3>& target, const Vector<T, 3>& up)
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
