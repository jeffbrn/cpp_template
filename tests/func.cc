#include <gtest/gtest.h>
#include "func.h"
#include <array>

using namespace  std;

namespace unit_tests {

TEST(FuncTest, Test1) {
	array<float,3> v { 1.f, 2.f, 3.f};
	auto result = calc_vec(v);
	EXPECT_EQ(result, 14.f);
}

}
