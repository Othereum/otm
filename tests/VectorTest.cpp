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
	EXPECT_FLOAT_EQ(v4.x, 0);
	EXPECT_FLOAT_EQ(v4.y, 0);
	EXPECT_FLOAT_EQ(v4.z, 1);
	EXPECT_FLOAT_EQ(v4.w, 1);

	v4.Assign(v1, -2);
	EXPECT_FLOAT_EQ(v4.x, 1);
	EXPECT_FLOAT_EQ(v4.y, 0);
	EXPECT_FLOAT_EQ(v4.z, 1);
	EXPECT_FLOAT_EQ(v4.w, 1);
}

TEST(VectorTest, Operations)
{
	Vec4 v1;
	EXPECT_THROW(v1.Normalize(), DivByZero);

	constexpr Vec3 v2{1, 1, 1};
	const Vec3 v3 = v2.Unit();
	constexpr auto f = 0.5773502691896258f;
	EXPECT_FLOAT_EQ(v3.x, f);
	EXPECT_FLOAT_EQ(v3.y, f);
	EXPECT_FLOAT_EQ(v3.z, f);

	EXPECT_FLOAT_EQ(v2.LenSqr(), 3);
	EXPECT_FLOAT_EQ(v2.Len(), 1.732050807568877f);

	constexpr Vec3 v4{3, 10, 2};
	EXPECT_FLOAT_EQ(v4.DistSqr(v2), 86);
	EXPECT_FLOAT_EQ(v4.Dist(v2), 9.273618495495704f);
}
