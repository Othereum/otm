#include <gtest/gtest.h>
#include "otm/Matrix.hpp"

namespace otm
{
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
	}

	TEST(MatrixTest, Operations)
	{
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
		
		auto mma = mmr;
		mma *= Matrix<long, 2, 2>::Identity();
		EXPECT_EQ(mma, mmr);

		constexpr auto mt = mm1.Transposed();
		static_assert(std::is_same_v<decltype(mt), const Matrix<int, 4, 2>>);
		constexpr Matrix<int, 4, 2> mte{
			1, 5,
			2, 6,
			3, 7,
			4, 8
		};
		EXPECT_EQ(mt, mte);
	}

	TEST(MatrixTest, Ext)
	{
		constexpr Vector v{2, 4, 5};
		constexpr auto rv = v.ToRowMatrix();
		constexpr auto cv = v.ToColMatrix();
		static_assert(std::is_same_v<decltype(rv), const Matrix<int, 1, 3>>);
		static_assert(std::is_same_v<decltype(cv), const Matrix<int, 3, 1>>);
		constexpr Matrix<int, 1, 3> rve{2, 4, 5};
		constexpr Matrix<int, 3, 1> cve{2, 4, 5};
		EXPECT_EQ(rv, rve);
		EXPECT_EQ(cv, cve);

		auto t1 = Matrix<int, 4, 4>::Identity(rv, {0, 3});
		constexpr Matrix<int, 4, 4> t1e{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			2, 4, 5, 1
		};
		EXPECT_EQ(t1, t1e);

		constexpr Mat3 m2{
			1, 2, 3,
			4, 5, 6,
			7, 8
		};
		t1.Assign(m2, {2, -1});
		constexpr Matrix<int, 4, 4> t2e{
			1, 0, 4, 5,
			0, 1, 7, 8,
			0, 0, 1, 0,
			2, 4, 5, 1
		};
		EXPECT_EQ(t1, t2e);
	}

	TEST(MatrixTest, DetInv)
	{
		constexpr Mat4 m{
			2, -1, 3, 0,
			-3, 1, 0, 4,
			-2, 1, 4, 1,
			-1, 3, 0, -2
		};
		constexpr auto md = m.Det();
		EXPECT_NEAR(md, -102_f, kSmallNum);

		for (auto i=0; i<10000; ++i)
		{
			const Matrix<double, 4> r{[]{ return Rand(0.1, 100); }};
			const auto im = r * r.Inv().value();
			ASSERT_TRUE(IsNearlyEqual(im, Matrix<double, 4>::identity));
		}
	}
}
