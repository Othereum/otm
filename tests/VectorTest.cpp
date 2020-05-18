#include <gtest/gtest.h>
#include "otm/Vector.hpp"

using namespace otm;

TEST(VectorTest, Constructor)
{
	Vec3 v1{1, 1, 1};
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 1);
	EXPECT_EQ(v1.z, 1);

	auto v2 = Vec3::One();
	EXPECT_EQ(v1, v2);

	Vec3 v3{All{}, 1};
	EXPECT_EQ(v2, v3);
}
