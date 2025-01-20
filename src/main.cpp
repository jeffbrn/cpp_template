#include <iostream>
#include "utils/AttitudeMath/matrix33.hpp"
#include "utils/AttitudeMath/dcm.h"

using namespace std;
using namespace utils::AttitudeMath;

int main() {
	cout << "Hello world!" << endl;

	auto mm = DCM::RotationX(0.3f);
	cout << mm << endl << endl;
	Matrix33<float> r = Matrix33<float>::Identity();

	for(int i = 0; i < 100; i++) {
		auto rdot = DCM::KinematicRates_BodyRate(r, Vector3<float>{1.f, 0.f, 0.f});
		r = DCM::Integrate(r, rdot, 0.01f);
		cout << "r:    " << r << "  - |" << r.isOrthogonal() << "|" << endl;
	}

	return 0;
}