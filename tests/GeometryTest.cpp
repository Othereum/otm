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

	const Quat q3{Vec3{0.369242f, -0.596525f, -0.712614f}.Unit(), 0.622533_rad};
	constexpr Quat q3e{0.1130857f, -0.1826944f, -0.2182483f, 0.9519464f};
	EXPECT_TRUE(IsNearlyEqual(q3, q3e));

	constexpr auto v1 = Vector<int, 3>::Forward();
	const auto v1r = v1.RotatedBy(Quat{UVec3::Right(), 90_deg});
	EXPECT_TRUE(IsNearlyEqual(v1r, Vec3::Down()));
	
	constexpr auto q3m = MakeRotation(q3e);
	constexpr Mat3 q3me{
		0.8379809f, -0.4568417f, 0.2984691f,
		0.3742011f, 0.8791586f, 0.2950486f,
		-0.3971921f, -0.1355576f, 0.9076688f
	};
	EXPECT_TRUE(IsNearlyEqual(q3m, q3me));
}

TEST(Geometry, Transform)
{
	constexpr Transform t1;
	EXPECT_TRUE(IsNearlyZero(t1.pos));
	EXPECT_TRUE(IsNearlyEqual(t1.rot, Quat{}));
	EXPECT_TRUE(IsNearlyEqual(t1.scale, Vec3::One()));

	constexpr auto t1m = t1.ToMatrix();
	EXPECT_TRUE(IsNearlyEqual(t1m, Mat4::Identity()));

	constexpr Quat q3e{0.1130857f, -0.1826944f, -0.2182483f, 0.9519464f};
	constexpr Transform t2{{-65, 24, 80}, q3e, {1.2, 1.2, 1}};
	constexpr auto t2m = t2.ToMatrix();
	constexpr Mat4 t2me{
		1.00557708f,	-0.54821004f,	0.35816292f,	0.f,
		0.44904132f,	1.05499032f,	0.35405832f,	0.f,
		-0.3971921f,	-0.1355576f,	0.9076688f,		0.f,
		-65.f,			24.f,			80.f,			1.f
	};
	EXPECT_TRUE(IsNearlyEqual(t2m, t2me));
}

TEST(Geometry, Angle)
{
	constexpr auto rad = 1.4_rad;
	constexpr Deg deg = rad;
	EXPECT_NEAR(deg.Get(), 80.21409132f, kSmallNum);
}

TEST(Geometry, QuatFromMat)
{
	for (auto i=0; i<100; ++i)
	{
		const auto q1 = Quat::Rand();
		const Quat q2{MakeRotation(q1)};
		ASSERT_TRUE(IsEquivalent(q1, q2));
	}
}

TEST(Geometry, DecompMatToTrsf)
{
	for (auto i=0; i<100; ++i)
	{
		const Transform trsf1{Vec3::Rand(-100, 100), Quat::Rand(), Vec3::Rand(0.1, 10)};
		const Transform trsf2{trsf1.ToMatrix()};
		
		ASSERT_TRUE(IsNearlyEqual(trsf1.pos, trsf2.pos));
		ASSERT_TRUE(IsEquivalent(trsf1.rot, trsf2.rot));
		ASSERT_TRUE(IsNearlyEqual(trsf1.scale, trsf2.scale));
	}
}
