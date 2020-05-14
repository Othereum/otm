#pragma once
#include <concepts>
#include <ratio>

namespace otm
{
	template <std::floating_point T>
	struct Quaternion;

	
	template <class T, size_t L>
	struct Vector;

	template <std::floating_point T, size_t L>
	struct UnitVec;

	using Vec2 = Vector<float, 2>;
	using Vec3 = Vector<float, 3>;
	using Vec4 = Vector<float, 4>;
	using Vec2u16 = Vector<uint16_t, 2>;
	using Vec3u16 = Vector<uint16_t, 3>;
	using UVec2 = UnitVec<float, 2>;
	using UVec3 = UnitVec<float, 3>;
	using UVec4 = UnitVec<float, 4>;

	
	template <class Ratio, class T = float>
	struct Angle;
	
	using PiRatio = std::ratio<5419351, 1725033>;
	
	using RadR = std::ratio<PiRatio::num, PiRatio::den * 180>;
	using DegR = std::ratio<1>;

	using Rad = Angle<RadR>;
	using Deg = Angle<DegR>;

	
	template <class T, size_t R, size_t C = R>
	struct Matrix;
	
	using Mat2 = Matrix<float, 2>;
	using Mat3 = Matrix<float, 3>;
	using Mat4 = Matrix<float, 4>;


	template <class... T>
	using CommonFloat = std::common_type_t<float, T...>;
}
