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
	EXPECT_NEAR(v4.x, 0, kSmallNumber);
	EXPECT_NEAR(v4.y, 0, kSmallNumber);
	EXPECT_NEAR(v4.z, 1, kSmallNumber);
	EXPECT_NEAR(v4.w, 1, kSmallNumber);

	v4.Assign(v1, -2);
	EXPECT_NEAR(v4.x, 1, kSmallNumber);
	EXPECT_NEAR(v4.y, 0, kSmallNumber);
	EXPECT_NEAR(v4.z, 1, kSmallNumber);
	EXPECT_NEAR(v4.w, 1, kSmallNumber);
}

TEST(VectorTest, Operations)
{
	Vec4 v1;
	EXPECT_THROW(v1.Normalize(), DivByZero);

	constexpr Vec3 v2{1, 1, 1};
	const Vec3 v3 = v2.Unit();
	constexpr auto f = 0.5773502691896258f;
	EXPECT_NEAR(v3.x, f, kSmallNumber);
	EXPECT_NEAR(v3.y, f, kSmallNumber);
	EXPECT_NEAR(v3.z, f, kSmallNumber);

	EXPECT_NEAR(v2.LenSqr(), 3, kSmallNumber);
	EXPECT_NEAR(v2.Len(), 1.732050807568877f, kSmallNumber);

	constexpr Vec3 v4{3, 10, 2};
	EXPECT_NEAR(v4.DistSqr(v2), 86, kSmallNumber);
	EXPECT_NEAR(v4.Dist(v2), 9.273618495495704f, kSmallNumber);
}
