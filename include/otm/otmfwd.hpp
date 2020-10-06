#pragma once
#include <concepts>

#ifndef OTM_DEFAULT_FLOAT
#define OTM_DEFAULT_FLOAT float
#endif

namespace otm
{
using Float = OTM_DEFAULT_FLOAT;
static_assert(std::is_floating_point_v<Float>);


template <class T, size_t L>
struct Vector;

template <std::floating_point T, size_t L>
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


template <class T, size_t Row, size_t Col = Row>
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
