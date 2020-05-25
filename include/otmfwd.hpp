#pragma once
#include <concepts>
#include <ratio>

#ifndef OTM_DEFAULT_FLOAT
	#define OTM_DEFAULT_FLOAT Float
#endif

namespace otm
{
	using Float = OTM_DEFAULT_FLOAT;
	
	template <std::floating_point T>
	struct Quaternion;

	using Quat = Quaternion<Float>;

	
	template <class T, size_t L>
	struct Vector;

	template <std::floating_point T, size_t L>
	struct UnitVec;

	using Vec2 = Vector<Float, 2>;
	using Vec3 = Vector<Float, 3>;
	using Vec4 = Vector<Float, 4>;
	using Vec2u16 = Vector<uint16_t, 2>;
	using Vec3u16 = Vector<uint16_t, 3>;
	using UVec2 = UnitVec<Float, 2>;
	using UVec3 = UnitVec<Float, 3>;
	using UVec4 = UnitVec<Float, 4>;

	
	template <class Ratio, class T = Float>
	struct Angle;
	
	using PiRatio = std::ratio<5419351, 1725033>;
	
	using RadR = std::ratio<PiRatio::num, PiRatio::den * 180>;
	using DegR = std::ratio<1>;

	using Rad = Angle<RadR>;
	using Deg = Angle<DegR>;

	
	template <class T, size_t R, size_t C = R>
	struct Matrix;
	
	using Mat2 = Matrix<Float, 2>;
	using Mat3 = Matrix<Float, 3>;
	using Mat4 = Matrix<Float, 4>;


	template <class... T>
	using CommonFloat = std::common_type_t<Float, T...>;
}
