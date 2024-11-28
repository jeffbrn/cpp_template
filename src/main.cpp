#include <iostream>
#include "func.h"

using namespace std;

int main() {
	cout << "Hello world!" << endl;

	array<float,3> v { 1.f, 2.f, 3.f};
	auto result = calc_vec(v);

	cout << "Distance of [" << v[0] << "," << v[1] << "," << v[2] << "] is " << result << endl;

	return 0;
}