#pragma once

#include "matrix33.hpp"
//#include <array>
#include <cmath>

namespace utils::AttitudeMath::DCM {

template<typename T>
T DegreesToRadians(T deg) {
	return deg * static_cast<T>(M_PI) / static_cast<T>(180);
}

template<typename T>
T RadiansToDegrees(T radian) {
	return radian / static_cast<T>(M_PI) * static_cast<T>(180);
}

template<typename T>
Vector3<T> DegreesToRadians(Vector3<T> deg) {
	return Vector3<T>(DegreesToRadians(deg[0]), DegreesToRadians(deg[1]), DegreesToRadians(deg[2]));
}

template<typename T>
Vector3<T> RadiansToDegrees(Vector3<T> radian) {
	return Vector3<T>(RadiansToDegrees(radian[0]), RadiansToDegrees(radian[1]), RadiansToDegrees(radian[2]));
}

template<typename T>
Vector3<T> DegreesToRadians(std::initializer_list<T> deg) {
	if (deg.size() != 3) throw std::invalid_argument("must be 3 elements in list");
	return Vector3<T>(DegreesToRadians(deg.begin()[0]), DegreesToRadians(deg.begin()[1]), DegreesToRadians(deg.begin()[2]));
}

template<typename T>
Vector3<T> RadiansToDegrees(std::initializer_list<T> radian) {
	if (radian.size() != 3) throw std::invalid_argument("must be 3 elements in list");
	return Vector3<T>(RadiansToDegrees(radian.begin()[0]), DegreesToRadians(radian.begin()[1]), DegreesToRadians(radian.begin()[2]));
}

/// @brief Return a rotation matrix for a rotation around the X-Axis
/// @param angle angle of rotation in radians
/// @return The rotation matrix
template<typename T>
Matrix33<T> RotationX(T angle);

/// @brief Return a rotation matrix for a rotation around the Y-Axis
/// @param angle angle of rotation in radians
/// @return The rotation matrix
template<typename T>
Matrix33<T> RotationY(T angle);

/// @brief Return a rotation matrix for a rotation around the Z-Axis
/// @param angle angle of rotation in radians
/// @return The rotation matrix
template<typename T>
Matrix33<T> RotationZ(T angle);

template<typename T>
Matrix33<T> Normalize(const Matrix33<T>& m) {
	auto x = m.row(0);
	auto y = m.row(1);
	auto error = x.dot(y);
	auto x_orth = x - (error * y / static_cast<T>(2));
	auto y_orth = y - (error * x / static_cast<T>(2));
	auto z_orth = x_orth.cross(y_orth);

	auto x_norm = (static_cast<T>(3) - x_orth.dot(x_orth)) / static_cast<T>(2) * x_orth;
	auto y_norm = (static_cast<T>(3) - y_orth.dot(y_orth)) / static_cast<T>(2) * y_orth;
	auto z_norm = (static_cast<T>(3) - z_orth.dot(z_orth)) / static_cast<T>(2) * z_orth;

	return Matrix33<T>(x_norm, y_norm, z_norm).transpose();
}

template<typename T>
Matrix33<T> Integrate(const Matrix33<T>& dcm, const Matrix33<T>& dcm_rates, T dt) {
	auto dcm_new = dcm + dcm_rates * dt; // 1st order euler integration
	return Normalize(dcm_new);
}

template<typename T>
Matrix33<T> KinematicRates_BodyRate(const Matrix33<T>& dcm, const Vector3<T>& body_rates) {
	const auto skew = Matrix33<T> {
		0, -body_rates.z(),body_rates.y(),
		body_rates.z(), 0, -body_rates.x(),
		-body_rates.y(), body_rates.x(), 0
	};
	return -skew * dcm;
}

template<typename T>
Matrix33<T> KinematicRates_WorldRate(const Matrix33<T>& dcm, const Vector3<T>& world_rates) {
	auto skew = Matrix33<T> {
		0, -world_rates.z(),world_rates.y(),
		world_rates.z(), 0, -world_rates.x(),
		-world_rates.y(), world_rates.x(), 0
	};
	return dcm * skew;
}

}
