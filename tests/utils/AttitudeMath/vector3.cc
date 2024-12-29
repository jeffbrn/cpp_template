#include <gtest/gtest.h>
#include <iostream>
#include "utils/AttitudeMath/vector3.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace utils::AttitudeMath {

template<typename T>
void PrintTo(const Vector3<T>& v, ostream* os) {
	*os << "(" << v.x() << "," << v.y() << "," << v.z() << ")";
}

}

namespace unit_tests::utils::AttitudeMath {

class Vector3Testing : public ::testing::Test {
protected:
	void SetUp() override {
	}
};

TEST_F(Vector3Testing, ConstAccess) {
	const Vector3<int> v = {1, 2, 3};
	EXPECT_EQ(1, v.x());
	EXPECT_EQ(2, v.y());
	EXPECT_EQ(3, v.z());
	EXPECT_EQ(1, v[0]);
	EXPECT_EQ(2, v[1]);
	EXPECT_EQ(3, v[2]);
	EXPECT_THROW(v[3], std::invalid_argument);
	EXPECT_THROW(v[-1], std::invalid_argument);
}

TEST_F(Vector3Testing, BadConstructor) {
	EXPECT_THROW(Vector3<int>({1, 2}), std::invalid_argument);
	EXPECT_THROW(Vector3<int>({1, 2, 3, 4}), std::invalid_argument);
}

TEST_F(Vector3Testing, ElementAccess) {
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
	EXPECT_THROW(v[3], std::invalid_argument);
	EXPECT_THROW(v[-1], std::invalid_argument);
}

TEST_F(Vector3Testing, OperatorAssignmentScalar) {
	Vector3<float> v1(1.f, 2.f, 3.f);
	v1 += 0.5;
	const Vector3<float> v2(1.5f, 2.5f, 3.5f);
	EXPECT_EQ(v1, v2);
	v1 -= 1.f;
	const Vector3<float> v3(0.5f, 1.5f, 2.5f);
	EXPECT_EQ(v1, v3);
	v1 *= 2.f;
	const Vector3<float> v4(1.f, 3.f, 5.f);
	EXPECT_EQ(v1, v4);
	v1 /= 2.f;
	EXPECT_EQ(v1, v3);
}

TEST_F(Vector3Testing, OperatorAssignmentVector) {
	Vector3<float> v1a(1.f, 2.f, 3.f);
	const Vector3<float> v1b(1.5f, 2.5f, 3.5f);
	const Vector3<float> v1r(2.5f, 4.5f, 6.5f);
	v1a += v1b;
	EXPECT_EQ(v1a, v1r);
	Vector3<float> v2a(1.f, 2.f, 3.f);
	const Vector3<float> v2b(1.5f, 2.5f, 3.5f);
	const Vector3<float> v2r(-.5f, -.5f, -.5f);
	v2a -= v2b;
	EXPECT_EQ(v2a, v2r);
	Vector3<float> v3a(1.5f, 2.5f, 3.5f);
	const Vector3<float> v3b(2.f, 4.f, 6.f);
	const Vector3<float> v3r(3.f, 10.f, 21.f);
	v3a *= v3b;
	EXPECT_EQ(v3a, v3r);
	Vector3<float> v4a(2.f, 3.f, 4.f);
	const Vector3<float> v4b(2.f, 3.f, 8.f);
	const Vector3<float> v4r(1.f, 1.f, 0.5f);
	v4a /= v4b;
	EXPECT_EQ(v4a, v4r);
}

}
