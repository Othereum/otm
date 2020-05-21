#include <gtest/gtest.h>
#include "otm/Matrix.hpp"

using namespace otm;

TEST(MatrixTest, Basic)
{
	constexpr Mat4 m1;
	for (auto& v : m1) for (auto f : v) EXPECT_EQ(f, 0);

	constexpr Mat3 m2{
		1, 2, 3,
		4, 5, 6,
		7, 8
	};
	EXPECT_EQ(m2[0][0], 1);
	EXPECT_EQ(m2[0][1], 2);
	EXPECT_EQ(m2[0][2], 3);
	EXPECT_EQ(m2[1][0], 4);
	EXPECT_EQ(m2[1][1], 5);
	EXPECT_EQ(m2[1][2], 6);
	EXPECT_EQ(m2[2][0], 7);
	EXPECT_EQ(m2[2][1], 8);
	EXPECT_EQ(m2[2][2], 0);

	constexpr Matrix<int, 2, 4> mm1{
		1, 2, 3, 4,
		5, 6, 7, 8
	};
	constexpr Matrix<long, 4, 2> mm2{
		8, 7,
		6, 5,
		4, 3,
		2, 1
	};
	[[maybe_unused]] constexpr auto mmr = mm1 * mm2;
	static_assert(std::is_same_v<decltype(mmr), const Matrix<long, 2, 2>>);
	constexpr Matrix<long, 2, 2> mmre{
		40, 30,
		120, 94
	};
	EXPECT_EQ(mmr, mmre);
}
