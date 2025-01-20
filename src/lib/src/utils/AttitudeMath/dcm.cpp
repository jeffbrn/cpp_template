#include "utils/AttitudeMath/dcm.h"

namespace utils::AttitudeMath::DCM {

template<>
Matrix33<float> RotationX(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		1.f, 0.f, 0.f,
		0.f,   c,   s,
		0.f,  -s,   c
	};
}

template<>
Matrix33<double> RotationX(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		1.0, 0.0, 0.0,
		0.0,   c,   s,
		0.0,  -s,   c
	};
}

template<>
Matrix33<float> RotationY(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		  c, 0.f,  -s,
		0.f, 1.f, 0.f,
		  s, 0.f, c
	};
}

template<>
Matrix33<double> RotationY(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		  c, 0.0,  -s,
		0.0, 1.0, 0.0,
		  s, 0.0, c
	};
}

template<>
Matrix33<float> RotationZ(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Matrix33<float> {
		  c,   s, 0.f,
		 -s,   c, 0.f,
		0.f, 0.f, 1.f,
	};
}

template<>
Matrix33<double> RotationZ(double angle) {
	double c = cos(angle);
	double s = sin(angle);
	return Matrix33<double> {
		  c,   s, 0.0,
		 -s,   c, 0.0,
		0.0, 0.0, 1.0,
	};
}

}
