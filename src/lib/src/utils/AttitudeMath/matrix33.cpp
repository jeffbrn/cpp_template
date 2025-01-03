#include "utils/AttitudeMath/matrix33.hpp"
#include <cmath>
#include "utils/AttitudeMath/matrix33.hpp"

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

template <>
Matrix33<double> &Matrix33<double>::inverse()
{
	auto det = determinant();
	if (det == 0.f) throw std::invalid_argument("Cannot invert a matrix with a zero determinant");
	auto invdet = 1.f / det;
	Matrix33<double> adj;
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

template<>
Matrix33<float> Matrix33<float>::RotationX(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		1.f, 0.f, 0.f,
		0.f,   c,   s,
		0.f,  -s,   c
	};
}

template<>
Matrix33<double> Matrix33<double>::RotationX(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		1.0, 0.0, 0.0,
		0.0,   c,   s,
		0.0,  -s,   c
	};
}

template<>
Matrix33<float> Matrix33<float>::RotationY(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		  c, 0.f,  -s,
		0.f, 1.f, 0.f,
		  s, 0.f, c
	};
}

template<>
Matrix33<double> Matrix33<double>::RotationY(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		  c, 0.0,  -s,
		0.0, 1.0, 0.0,
		  s, 0.0, c
	};
}

template<>
Matrix33<float> Matrix33<float>::RotationZ(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		  c,   s, 0.f,
		 -s,   c, 0.f,
		0.f, 0.f, 1.f,
	};
}

template<>
Matrix33<double> Matrix33<double>::RotationZ(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		  c,   s, 0.0,
		 -s,   c, 0.0,
		0.0, 0.0, 1.0,
	};
}

}
