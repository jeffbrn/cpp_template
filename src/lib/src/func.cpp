#include "func.h"
#include <Eigen/Dense>

using namespace Eigen;

float calc_vec(std::array<float,3> data) {
	Vector3f v(data[0], data[1], data[2]);
	return v.squaredNorm();
}
