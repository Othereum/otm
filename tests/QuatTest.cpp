#include <gtest/gtest.h>
#include "otm/Quat.hpp"

using namespace otm;

TEST(QuatTest, Basic)
{
	constexpr Quat q1;
	EXPECT_TRUE(IsNearlyEqual(q1.v, Vec3{}));
	EXPECT_EQ(q1.s, 1);

	constexpr auto q2 = q1 * Quat{};
	EXPECT_TRUE(IsNearlyEqual(q1, q2));

	constexpr auto v1 = Vector<int, 3>::Forward();
	const auto v1r = v1.RotatedBy(Quat{UVec3::Right(), 90_deg});
	EXPECT_TRUE(IsNearlyEqual(v1r, Vec3::Down()));
}
