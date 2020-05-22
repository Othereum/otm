#include <gtest/gtest.h>
#include "otm/Transform.hpp"

using namespace otm;

TEST(Geometry, Quat)
{
	constexpr Quat q1;
	EXPECT_TRUE(IsNearlyEqual(q1.v, Vec3{}));
	EXPECT_EQ(q1.s, 1);

	constexpr auto q2 = q1 * Quat{};
	EXPECT_TRUE(IsNearlyEqual(q1, q2));

	Quat q3{Vec3{0.369242f, -0.596525f, -0.712614f}.Unit(), 0.622533_rad};
	constexpr Quat q3e{0.1130857f, -0.1826944f, -0.2182483f, 0.9519464f};
	EXPECT_TRUE(IsNearlyEqual(q3, q3e));

	constexpr auto v1 = Vector<int, 3>::Forward();
	const auto v1r = v1.RotatedBy(Quat{UVec3::Right(), 90_deg});
	EXPECT_TRUE(IsNearlyEqual(v1r, Vec3::Down()));
}

TEST(Geometry, Transform)
{
	constexpr Transform t1;
	EXPECT_TRUE(IsNearlyZero(t1.pos));
	EXPECT_TRUE(IsNearlyEqual(t1.rot, Quat{}));
	EXPECT_TRUE(IsNearlyEqual(t1.scale, Vec3::One()));

	constexpr auto t1m = t1.ToMatrix();
	EXPECT_TRUE(IsNearlyEqual(t1m, Mat4::Identity()));
}
