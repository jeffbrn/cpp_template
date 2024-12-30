#include "utils/AttitudeMath/matrix33.hpp"
#include <cmath>

using namespace std;

namespace utils::AttitudeMath {

template<>
Matrix33<float>& Matrix33<float>::inverse() {
	auto det = determinant();
	if (det == 0.f) throw std::invalid_argument("Cannot invert a matrix with a zero determinant");
	auto invdet = 1.f / det;
	Matrix33<float> adj;
	adj(0,0) = _elem[4] * _elem[8] - _elem[5] * _elem[7];
	adj(0,1) = _elem[2] * _elem[7] - _elem[1] * _elem[8];
	adj(0,2) = _elem[1] * _elem[5] - _elem[2] * _elem[4];
	adj(1,0) = _elem[5] * _elem[6] - _elem[3] * _elem[8];
	adj(1,1) = _elem[0] * _elem[8] - _elem[2] * _elem[6];
	adj(1,2) = _elem[2] * _elem[3] - _elem[0] * _elem[5];
	adj(2,0) = _elem[3] * _elem[7] - _elem[4] * _elem[6];
	adj(2,1) = _elem[1] * _elem[6] - _elem[0] * _elem[7];
	adj(2,2) = _elem[0] * _elem[4] - _elem[1] * _elem[3];
	*this = adj * invdet;
	return *this;
}

}
