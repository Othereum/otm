#pragma once
#include <cstddef>
#include <ratio>
#include <type_traits>

#ifndef OTM_DEFAULT_FLOAT
#define OTM_DEFAULT_FLOAT float
#endif

namespace otm
{
using Float = OTM_DEFAULT_FLOAT;
static_assert(std::is_floating_point_v<Float>);

constexpr Float operator""_f(unsigned long long f) noexcept
{
    return static_cast<Float>(f);
}

constexpr Float operator""_f(long double f) noexcept
{
    return static_cast<Float>(f);
}

template <class... T>
using CommonFloat = std::common_type_t<Float, T...>;


struct Transform;

template <class T>
struct Quaternion;

using Quat = Quaternion<Float>;


template <class T, size_t L>
struct Vector;

template <class T, size_t L>
struct UnitVec;

using Vec2 = Vector<Float, 2>;
using Vec3 = Vector<Float, 3>;
using Vec4 = Vector<Float, 4>;

using Vec2i = Vector<int32_t, 2>;
using Vec2u = Vector<uint32_t, 2>;
using Vec3i = Vector<int32_t, 3>;
using Vec3u = Vector<uint32_t, 3>;
using Vec4i = Vector<int32_t, 4>;
using Vec4u = Vector<uint32_t, 4>;

using Vec2i16 = Vector<int16_t, 2>;
using Vec2u16 = Vector<uint16_t, 2>;
using Vec3i16 = Vector<int16_t, 3>;
using Vec3u16 = Vector<uint16_t, 3>;
using Vec4i16 = Vector<int16_t, 4>;
using Vec4u16 = Vector<uint16_t, 4>;

using UVec2 = UnitVec<Float, 2>;
using UVec3 = UnitVec<Float, 3>;
using UVec4 = UnitVec<Float, 4>;


template <class Ratio, class T = Float>
struct Angle;

using PiRatio = std::ratio<66627445592888887, 21208174623389167>;

using RadR = std::ratio<PiRatio::num, PiRatio::den * 180>;
using DegR = std::ratio<1>;

using Rad = Angle<RadR>;
using Deg = Angle<DegR>;


template <class T, size_t R, size_t C = R>
struct Matrix;

using Mat2 = Matrix<Float, 2>;
using Mat3 = Matrix<Float, 3>;
using Mat4 = Matrix<Float, 4>;
using Mat2x3 = Matrix<Float, 2, 3>;
using Mat2x4 = Matrix<Float, 2, 4>;
using Mat3x2 = Matrix<Float, 3, 2>;
using Mat3x4 = Matrix<Float, 3, 4>;
using Mat4x2 = Matrix<Float, 4, 2>;
using Mat4x3 = Matrix<Float, 4, 3>;
} // namespace otm
