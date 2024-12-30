#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "utils/AttitudeMath/matrix33.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace utils::AttitudeMath {

/// @brief Allows gogletest to print vector value
template<typename T>
void PrintTo(const Matrix33<T>& m, ostream* os) {
	*os << endl << "[ "
		<< m(0,0) << "," << m(0,1) << "," << m(0,2) << ";" << endl << "  "
		<< m(1,0) << "," << m(1,1) << "," << m(1,2) << ";" << endl << "  "
		<< m(2,0) << "," << m(2,1) << "," << m(2,2) << ";" << endl
		<< " ]";
}

}

namespace unit_tests::utils::AttitudeMath {

class Matrix33Testing : public ::testing::Test {
protected:
	void SetUp() override {
	}
};

TEST_F(Matrix33Testing, ElementAccess) {
	Matrix33<int> m;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			EXPECT_EQ(0, m(i,j));
			m(i,j) = i * 10 + j;
			EXPECT_EQ(i * 10 + j, m(i,j));
		}
	}
}

TEST_F(Matrix33Testing, Constructors) {
	Matrix33<float> i = Matrix33<float>(Vector3<float>::xAxis(), Vector3<float>::yAxis(), Vector3<float>::zAxis());
	EXPECT_EQ(i, Matrix33<float>::Identity());
	Matrix33<double> m1 { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	Matrix33<double> m2(m1);
	EXPECT_EQ(m1, m2);
	m2(1,1) = 45.0;
	EXPECT_FALSE(m1 == m2);
}

TEST_F(Matrix33Testing, OperatorAssignmentsScalar) {
	Matrix33<int> m1a { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Matrix33<int> m1b { 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	m1a += 10;
	EXPECT_EQ(m1a, m1b);
	Matrix33<float> m2a { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
	Matrix33<float> m2b { 1.5f, 3.f, 4.5f, 6.f, 7.5f, 9.f, 10.5f, 12.f, 13.5f };
	m2a *= 1.5f;
	EXPECT_EQ(m2a, m2b);
	Matrix33<double> m3a { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	Matrix33<double> m3b { 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25 };
	m3a /= 4.0;
	EXPECT_THROW(m3a /= 0.0, std::invalid_argument);
	EXPECT_EQ(m3a, m3b);
	Matrix33<int> m4a = Matrix33<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Matrix33<int> m4b = Matrix33<int> { -9, -8, -7, -6, -5, -4, -3, -2, -1 };
	m4a -= 10;
	EXPECT_EQ(m4a, m4b);
}

TEST_F(Matrix33Testing, OperatorAssignmentsMatrix) {
	Matrix33<int> m1a {  1,  2,  3,  4,  5,  6,  7,  8,  9 };
	Matrix33<int> m1b { 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	Matrix33<int> m1c { 12, 14, 16, 18, 20, 22, 24, 26, 28 };
	m1a += m1b;
	EXPECT_EQ(m1a, m1c);
	Matrix33<double> m2a {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0 };
	Matrix33<double> m2b { 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };
	Matrix33<double> m2c {-10.0,-10.0,-10.0,-10.0,-10.0,-10.0,-10.0,-10.0,-10.0 };
	m2a -= m2b;
	EXPECT_EQ(m2a, m2c);
}

TEST_F(Matrix33Testing, MatrixMatrixOps) {
	Matrix33<int> m1a {  1,  2,  3,  4,  5,  6,  7,  8,  9 };
	auto m1b = -m1a;
	EXPECT_EQ(m1a + m1b, Matrix33<int>());
	Matrix33<float> m2a { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
	Matrix33<float> m2b(m2a);
	EXPECT_EQ(m2a - m2b, Matrix33<float>());
	Matrix33<int> m3a {  1,  2,  3,  4,  5,  6,  7,  8,  9 };
	Matrix33<int> m3b {  9, 8, 7, 6, 5, 4, 3, 2, 1 };
	Matrix33<int> m3c {  30, 24, 18, 84, 69, 54, 138, 114, 90 };
	EXPECT_EQ(m3a * m3b, m3c);
	EXPECT_EQ(m3c * Matrix33<int>::Identity(), m3c);
	Vector3<int> v3a { 1, 2, 3 };
	EXPECT_EQ(m3a * v3a, Vector3<int>(14, 32, 50));
}

TEST_F(Matrix33Testing, MatrixScalarOps) {
	const Matrix33<int> m1a { 1, 2, 3, 4, 5, 6, 7, 8,  9 };
	const Matrix33<int> m1b { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	EXPECT_EQ(m1a + 1, m1b);
	EXPECT_EQ(1 + m1a, m1b);
	const Matrix33<float> m2a { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
	const Matrix33<float> m2b { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };
	EXPECT_EQ(1.f - m2a, -m2b);
	const Matrix33<double> m3a { 1.0, 2.0, 3.0, 4.0,  5.0,  6.0,  7.0,  8.0,  9.0 };
	const Matrix33<double> m3b { 2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0 };
	EXPECT_EQ(m3a * 2.0, m3b);
	EXPECT_EQ(2.0 * m3a, m3b);
	const Matrix33<float> m4a {  1.f, 2.f,  3.f, 4.f,  5.f, 6.f,  7.f, 8.f,  9.f };
	const Matrix33<float> m4b { 0.5f, 1.f, 1.5f, 2.f, 2.5f, 3.f, 3.5f, 4.f, 4.5f };
	EXPECT_EQ(m4a / 2.f, m4b);
	EXPECT_THROW(m3a / 0.0, std::invalid_argument);
	Matrix33<double> m5a { 2.0, 4.0, 6.0, 0.0, 10.0, 12.0, 14.0, 16.0, 18.0 };
	EXPECT_THROW(10.0 / m5a, std::invalid_argument);
}

TEST_F(Matrix33Testing, MatrixDiagonal) {
	const Matrix33<int> m1a { 1, 2, 3, 4, 5, 6, 7, 8,  9 };
	auto d = m1a.diag();
	EXPECT_EQ(d, Vector3<int>(1, 5, 9));
	const Matrix33<int> m2a(d);
	const Matrix33<int> m2b { 1, 0, 0, 0, 5, 0, 0, 0, 9 };
	EXPECT_EQ(m2a, m2b);
}

TEST_F(Matrix33Testing, MatrixOperations) {
	Matrix33<int> m1a { 1, 2, 3, 4, 5, 6, 7, 8,  9 };
	auto m1b = transpose(m1a);
	Matrix33<int> m1c(m1a);
	EXPECT_EQ(m1b, m1a.transpose());
	EXPECT_EQ(m1c, m1a.transpose());
	EXPECT_EQ(m1a.determinant(), 0);
	EXPECT_EQ(Matrix33<float>::Identity().determinant(), 1);
	Matrix33<float> m2a { 1.f, 2.f, -1.f, 2.f, 1.f, 2.f, -1.f, 2.f, 1.f };
	Matrix33<float> m2b(m2a);
	EXPECT_EQ(m2a * m2b.inverse(), Matrix33<float>::Identity());
}

}
