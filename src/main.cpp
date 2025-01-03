#include <iostream>
#include "utils/AttitudeMath/matrix33.hpp"

using namespace std;
using namespace utils::AttitudeMath;

int main() {
	cout << "Hello world!" << endl;

	Matrix33<float> r = Matrix33<float>::Identity();

	for(int i = 0; i < 100; i++) {
		auto rdot = r.dcmKinematicRates_BodyRate(Vector3<float>{1.f, 0.f, 0.f});
		r = r.integrateDCM(rdot, 0.01f);
		cout << "r:    " << r << "  - |" << r.isOrthogonal() << "|" << endl;
	}

	return 0;
}