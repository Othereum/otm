#include <gtest/gtest.h>
#include "otm/Matrix.hpp"

using namespace otm;
constexpr auto kCnt = 1'000'000;

TEST(Benchmark, Float)
{
	constexpr Matrix<float, 4, 4> m{All{}, 1.1f};
	for (auto i=0; i<kCnt; ++i) (void)(m * m);
}

TEST(Benchmark, Double)
{
	constexpr Matrix<double, 4, 4> m{All{}, 1.1};
	for (auto i=0; i<kCnt; ++i) (void)(m * m);
}
