#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "utils/AttitudeMath/matrix33.hpp"
#include "test_stream_formatters.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace unit_tests::utils::AttitudeMath {

class DcmTesting : public ::testing::Test {
protected:
	void SetUp() override {
	}
};

TEST_F(DcmTesting, SingleAccessRotations) {
	auto rx = Matrix33<float>::RotationX(30.f*M_PI/180.f);
	Matrix33<float> rx_chk {
		1.f, 0.f, 0.f,
		0.f, 0.8660254f, 0.5f,
		0.f, -0.5f, 0.8660254f
	};
	EXPECT_EQ(rx, rx_chk);
	auto ry = Matrix33<double>::RotationY(30.0*M_PI/180.0);
	Matrix33<double> ry_chk {
		  0.866025, 0.0,  -0.5,
		0.0, 1.0, 0.0,
		0.5, 0.0, 0.866025
	};
	Test_NearEqual(ry, ry_chk);
	auto rz = Matrix33<float>::RotationZ(30.f*M_PI/180.f);
	Matrix33<float> rz_chk {
		0.8660254f, 0.5f, 0.f,
		-0.5f, 0.8660254f, 0.f,
		0.f, 0.f, 1.f,
	};
	EXPECT_EQ(rz, rz_chk);
}

// TEST_F(DcmTesting, Orthoganality) {
// 	auto m = Matrix33<float>::RotationX(30.f*M_PI/180.f);
// 	EXPECT_TRUE(m.isOrthogonal());
// 	auto m_inv = inverse(m);
// 	auto m_t = transpose(m);
// 	EXPECT_EQ(m_inv, m_t);
// 	EXPECT_EQ(m * m_t, Matrix33<float>::Identity());

// 	auto rot = Matrix33<double>::RotationX(30.0*M_PI/180.0) * Matrix33<double>::RotationY(30.0*M_PI/180.0) * Matrix33<double>::RotationZ(30.0*M_PI/180.0);
// 	Test_NearEqual(inverse(rot), transpose(rot));
// 	EXPECT_TRUE(rot.isOrthogonal());

// 	auto not_oth = Matrix33<float> {
// 		1.f, 2.f, 3.f,
// 		4.f, 5.f, 6.f,
// 		7.f, 8.f, 9.f
// 	};
// 	EXPECT_FALSE(not_oth.isOrthogonal());
// }

TEST_F(DcmTesting, Normalize) {
	auto m1 = Matrix33<float>::RotationX(30.f*M_PI/180.f);
	Matrix33<float> m2 = m1.normalizeDCM();
	//EXPECT_TRUE(m2.isOrthogonal());
	m1(1,2) = -m1(1,2);
	m1(2,1) = -m1(2,1);
	Test_NearEqual(m1, m2);
	auto not_oth = Matrix33<float> {
		1.f, 2.f, 3.f,
		4.f, 5.f, 6.f,
		7.f, 8.f, 9.f
	};
	EXPECT_THROW(not_oth.normalizeDCM(), std::invalid_argument);
}

}
