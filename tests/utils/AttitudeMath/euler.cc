#include <gtest/gtest.h>
#include <iostream>
#include "utils/AttitudeMath/euler_angles.hpp"
#include "test_stream_formatters.hpp"

using namespace std;
using namespace utils::AttitudeMath;

namespace unit_tests::utils::AttitudeMath {

class EulerTesting : public ::testing::Test {
protected:
	void SetUp() override {
	}
};

TEST_F(EulerTesting, TestRotation) {
	Vector3 angles_deg(-30.0, 65.0, -45.0);
	auto ang = DCM::DegreesToRadians(angles_deg);
	Euler::EulerAngles<double> e(ang[0], ang[1], ang[2]);

	Matrix33<double> expected { 0.29883624, -0.29883624,-0.90630779, 0.29194424, 0.93280063, -0.21130913, 0.9085513, -0.20144452, 0.36599815 };
	Test_NearEqual(e.get_dcm(), expected, 0.0000001);

	auto result = DCM::RadiansToDegrees(e.get_angles());
	EXPECT_EQ(angles_deg, result);
}

TEST_F(EulerTesting, ChangeFoR) {
	Euler::EulerAngles a2w(DCM::DegreesToRadians({35.f, 10.f, 75.f}));
	Euler::EulerAngles c2a(DCM::DegreesToRadians({0.f, -35.f, 0.f}));
	auto Rcw = c2a.get_dcm() * a2w.get_dcm();
	Euler::EulerAngles c2w(Rcw);
	auto attitude_cw = DCM::RadiansToDegrees(c2w.get_angles());
	cout << attitude_cw << endl;
	Vector3<float> expected(36.606189f, -18.6909897f, 54.67733444f);
	Test_NearEqual(attitude_cw, expected, 0.0001f);
}

}
