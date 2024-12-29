#include <gtest/gtest.h>
#include "utils/AttitudeMath/vector3.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace unit_tests::utils::AttitudeMath {

class Vector3Testing : public ::testing::Test {
protected:
	void SetUp() override {
	}
};

TEST_F(Vector3Testing, Vector3ConstAccess) {
	const Vector3<int> v(1, 2, 3);
	EXPECT_EQ(1, v.x());
	EXPECT_EQ(2, v.y());
	EXPECT_EQ(3, v.z());
	EXPECT_EQ(1, v[0]);
	EXPECT_EQ(2, v[1]);
	EXPECT_EQ(3, v[2]);
}

TEST_F(Vector3Testing, Vector3Access) {
	Vector3<int> v(1, 2, 3);
	EXPECT_EQ(1, v.x());
	EXPECT_EQ(2, v.y());
	EXPECT_EQ(3, v.z());
	v.x(4);
	v.y(5);
	v.z(6);
	EXPECT_EQ(4, v[0]);
	EXPECT_EQ(5, v[1]);
	EXPECT_EQ(6, v[2]);
	v[0] = 7;
	v[1] = 8;
	v[2] = 9;
	EXPECT_EQ(7, v.x());
	EXPECT_EQ(8, v.y());
	EXPECT_EQ(9, v.z());
}

}
