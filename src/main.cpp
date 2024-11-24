#include <iostream>
#include "func.h"

using namespace std;

int main() {
	cout << "Hello world!" << endl;

	Eigen::Vector3f v;
	v << 1.f, 2.f, 3.f;
	auto result = dist(v);

	cout << "Distance of " << v.transpose() << " is " << result << endl;

	return 0;
}