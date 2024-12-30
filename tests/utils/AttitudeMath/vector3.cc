#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "utils/AttitudeMath/vector3.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace utils::AttitudeMath {

/// @brief Allows gogletest to print vector value
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

TEST_F(Vector3Testing, PredefVectors) {
	const Vector3<double> v1a;
	const Vector3<double> v1b(0.0, 0.0, 0.0);
	EXPECT_EQ(v1a, v1b);
	auto v2a = Vector3<float>::xAxis();
	const Vector3<float> v2b(1.f, 0.f, 0.f);
	EXPECT_EQ(v2a, v2b);
	auto v3a = Vector3<int>::yAxis();
	const Vector3<int> v3b(0, 1, 0);
	EXPECT_EQ(v3a, v3b);
	auto v4a = Vector3<double>::zAxis();
	const Vector3<double> v4b(0.0, 0.0, 1.0);
	EXPECT_EQ(v4a, v4b);
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
	EXPECT_THROW(v1 /= 0.f, std::invalid_argument);
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
	EXPECT_THROW(v1a / Vector3<float>(), std::invalid_argument);
}

TEST_F(Vector3Testing, VectorVectorOps) {
	auto v1a = Vector3<int>::xAxis() + Vector3<int>::yAxis() + Vector3<int>::zAxis();
	const Vector3<int> v1b(1, 1, 1);
	EXPECT_EQ(v1a, v1b);
	auto v2a = Vector3<float>::xAxis() - Vector3<float>::yAxis() - Vector3<float>::zAxis();
	const Vector3<float> v2b(1.f, -1.f, -1.f);
	EXPECT_EQ(v2a, v2b);
	auto v3a = Vector3<int>::xAxis() * Vector3<int>::yAxis() * Vector3<int>::zAxis();
	const Vector3<int> v3b;
	EXPECT_EQ(v3a, v3b);
	auto v4a = Vector3<double>(1.0, 2.0, 3.0) / Vector3<double>(2.0, 2.0, 2.0);
	const Vector3<double> v4b(0.5, 1.0, 1.5);
	EXPECT_EQ(v4a, v4b);
	auto v5a = -Vector3<int>::xAxis();
	auto v5b = Vector3<int>(-1, 0, 0);
	EXPECT_EQ(v5a, v5b);
	EXPECT_THROW(v4b / Vector3<double>(), std::invalid_argument);
}

TEST_F(Vector3Testing, VectorScalarOps) {
	auto v1a = 1 + Vector3<int>::xAxis() - 2;
	const Vector3<int> v1b(0, -1, -1);
	EXPECT_EQ(v1a, v1b);
	auto v2a = 2.0 * Vector3<double>(2.0, 3.0, 4.0) / 4.0;
	const Vector3<double> v2b(1.0, 1.5, 2.0);
	EXPECT_EQ(v2a, v2b);
	EXPECT_THROW(v1b / 0, std::invalid_argument);
	EXPECT_THROW(4.0 / Vector3<double>(), std::invalid_argument);
}

TEST_F(Vector3Testing, VectorOperations) {
	const Vector3<int> v1(3, 4, 5);
	EXPECT_EQ(static_cast<int>(sqrt(50.0)), v1.norm());
	Vector3<float> v2(1.f, 2.f, 3.f);
	EXPECT_EQ(sqrt(14.f), v2.norm());
	auto v2_unit = v2.unit();
	v2.normalize();
	EXPECT_EQ(v2, Vector3<float>(1.f/sqrt(14.f), 2.f/sqrt(14.f), 3.f/sqrt(14.f)));
	EXPECT_EQ(v2_unit, v2);
	Vector3<double> v3(10.0, 20.0, 30.0);
	auto v3_norm = v3.norm();
	EXPECT_EQ(v3_norm, v3.norm());
	v3.normalize();
	EXPECT_EQ(v3, Vector3<double>(10.0/v3_norm, 20.0/v3_norm, 30.0/v3_norm));
	EXPECT_THROW(Vector3<double>().normalize(), std::invalid_argument);
	EXPECT_THROW(Vector3<double>().unit(), std::invalid_argument);
}

}
