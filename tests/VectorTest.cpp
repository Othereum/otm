#include <gtest/gtest.h>
#include "otm/Vector.hpp"

using namespace otm;

TEST(VectorTest, Basic)
{
	constexpr Vec4 v1{1, 1, 1};
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 1);
	EXPECT_EQ(v1.z, 1);
	EXPECT_EQ(v1.w, 0);

	constexpr Vec4 v2{All{}, 1};
	EXPECT_NE(v1, v2);

	constexpr Vec4 v3{Vec3{v1}, 1};
	EXPECT_EQ(v2, v3);

	Vec4 v4;
	v4.Assign(v1, 2);
	EXPECT_NEAR(v4.x, 0, kSmallNum);
	EXPECT_NEAR(v4.y, 0, kSmallNum);
	EXPECT_NEAR(v4.z, 1, kSmallNum);
	EXPECT_NEAR(v4.w, 1, kSmallNum);

	v4.Assign(v1, -2);
	EXPECT_NEAR(v4.x, 1, kSmallNum);
	EXPECT_NEAR(v4.y, 0, kSmallNum);
	EXPECT_NEAR(v4.z, 1, kSmallNum);
	EXPECT_NEAR(v4.w, 1, kSmallNum);
}

TEST(VectorTest, Operations)
{
	Vec4 v1;
	EXPECT_THROW(v1.Normalize(), DivByZero);
	EXPECT_FALSE(v1.Normalize(std::nothrow));
	EXPECT_THROW((void)v1.Unit(), DivByZero);
	EXPECT_FALSE(v1.Unit(std::nothrow).has_value());
	
	constexpr Vec3 v2{1, 1, 1};
	const Vec3 v3 = v2.Unit();
	constexpr auto f = 0.5773502691896258f;
	auto pred = [f](float x){ return IsNearlyEqual(x, f); };
	EXPECT_TRUE(std::all_of(v3.begin(), v3.end(), pred));

	EXPECT_NEAR(v2.LenSqr(), 3, kSmallNum);
	EXPECT_NEAR(v2.Len(), 1.732050807568877f, kSmallNum);

	constexpr Vec3 v4{3, 10, 2};
	EXPECT_NEAR(v4.DistSqr(v2), 86, kSmallNum);
	EXPECT_NEAR(v4.Dist(v2), 9.273618495495704f, kSmallNum);

	constexpr Vector cp1{2, 5, 1}, cp2{3, 1, 7};
	constexpr auto cpr = cp1 ^ cp2;
	constexpr Vector cpe{34, -11, -13};
	EXPECT_EQ(cpr, cpe);
}

TEST(VectorTest, Ext)
{
	const auto v1 = Vec3::Rand(0, 1);
	auto pred = [](float f) { return f >= 0 && f < 1; };
	EXPECT_TRUE(std::all_of(v1.begin(), v1.end(), pred));
	
	auto v2 = UVec3::Rand();
	EXPECT_NEAR(v2.Get().LenSqr(), 1, kSmallNum);

	float f1, f2, f3;
	v1 >> f1 >> f2 >> f3;
	EXPECT_EQ(v1.x, f1);
	EXPECT_EQ(v1.y, f2);
	EXPECT_EQ(v1.z, f3);

	Vec3 v3;
	v3 << f1 << f2 << f3;
	EXPECT_EQ(v1, v3);
}
