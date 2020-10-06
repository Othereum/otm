#include "otm/Vector.hpp"
#include <gtest/gtest.h>

namespace otm
{
TEST(VectorTest, Basic)
{
    constexpr Vec4 v1{1, 1, 1};
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 1);
    EXPECT_EQ(v1[2], 1);
    EXPECT_EQ(v1[3], 0);

    constexpr Vec4 v2{All{}, 1};
    EXPECT_FALSE(IsNearlyEqual(v1, v2));
    EXPECT_TRUE(IsNearlyEqual(v2, Vec4{1, 1, 1, 1}));

    constexpr auto vone = Vec3::One();
    EXPECT_EQ(vone[0], 1);
    EXPECT_EQ(vone[1], 1);
    EXPECT_EQ(vone[2], 1);

    constexpr Vec4 v3{Vec3{v1}, 1};
    EXPECT_TRUE(IsNearlyEqual(v2, v3));

    Vec4 v4;
    v4.Assign(v1, 2);
    EXPECT_NEAR(v4[0], 0, kSmallNum);
    EXPECT_NEAR(v4[1], 0, kSmallNum);
    EXPECT_NEAR(v4[2], 1, kSmallNum);
    EXPECT_NEAR(v4[3], 1, kSmallNum);

    v4.Assign(v1, -2);
    EXPECT_NEAR(v4[0], 1, kSmallNum);
    EXPECT_NEAR(v4[1], 0, kSmallNum);
    EXPECT_NEAR(v4[2], 1, kSmallNum);
    EXPECT_NEAR(v4[3], 1, kSmallNum);
}

TEST(VectorTest, Operations)
{
    Vec4 v1;
    EXPECT_THROW(v1.Normalize(), DivByZero);
    EXPECT_FALSE(v1.TryNormalize());
    EXPECT_FALSE(v1.Unit().has_value());

    constexpr Vec3 v2{1, 1, 1};
    const Vec3 v3 = *v2.Unit();
    constexpr auto f = 0.5773502691896258f;
    auto pred = [f](Float x) { return IsNearlyEqual(x, f); };
    EXPECT_TRUE(std::all_of(v3.begin(), v3.end(), pred));

    EXPECT_NEAR(v2.LenSqr(), 3, kSmallNum);
    EXPECT_NEAR(v2.Len(), 1.732050807568877f, kSmallNum);

    constexpr Vec3 v4{3, 10, 2};
    EXPECT_NEAR(v4.DistSqr(v2), 86, kSmallNum);
    EXPECT_NEAR(v4.Dist(v2), 9.273618495495704f, kSmallNum);

    constexpr Vector dp1{1, 3}, dp2{6, 2};
    constexpr auto dpr = dp1 | dp2;
    EXPECT_EQ(dpr, 12);

    constexpr Vector cp1{2, 5, 1}, cp2{3, 1, 7};
    constexpr auto cpr = cp1 ^ cp2;
    constexpr Vector cpe{34, -11, -13};
    EXPECT_EQ(cpr, cpe);
}

TEST(VectorTest, Ext)
{
    const auto v1 = Vec3::Rand(0, 1);
    auto pred = [](Float f) { return f >= 0 && f < 1; };
    EXPECT_TRUE(std::all_of(v1.begin(), v1.end(), pred));

    auto v2 = UVec3::Rand();
    EXPECT_NEAR(v2.Get().LenSqr(), 1, kSmallNum);

    Float f1, f2, f3;
    v1 >> f1 >> f2 >> f3;
    EXPECT_EQ(v1[0], f1);
    EXPECT_EQ(v1[1], f2);
    EXPECT_EQ(v1[2], f3);

    Vec3 v3;
    v3 << f1 << f2 << f3;
    EXPECT_TRUE(IsNearlyEqual(v1, v3));

    EXPECT_THROW((void)v3.at(3), std::out_of_range);
}
} // namespace otm
