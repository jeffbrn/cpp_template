#pragma once

#include "utils/AttitudeMath/matrix33.hpp"
#include "utils/AttitudeMath/dcm.h"

namespace utils::AttitudeMath::Euler {

template <typename T>
class EulerAngles {
public:
	EulerAngles(T roll, T pitch, T yaw) : _dcm(RotationXYZ(roll, pitch, yaw)), _angles(roll, pitch, yaw) {}
	EulerAngles(Vector3<T> angles) : _dcm(RotationXYZ(angles[0], angles[1], angles[2])), _angles { angles } {}
	EulerAngles(Matrix33<T> dcm) : _dcm { dcm }, _angles { CalcAngles() } {}

	/// @brief Return the angles in radians (roll, pitch, yaw) or (phi, theta, psi)
	/// @return 
	Vector3<T> get_angles() const { return _angles; }
	Matrix33<T> get_dcm() const { return _dcm; }

private:
	const Matrix33<T> _dcm;
	const Vector3<T> _angles;

	static Matrix33<T> RotationXYZ(T roll, T pitch, T yaw) {
		auto Rx = DCM::RotationX(roll);
		auto Ry = DCM::RotationY(pitch);
		auto Rz = DCM::RotationZ(yaw);
		return Rx * Ry * Rz;
	}
	Vector3<T> CalcAngles() {
		T phi = atan2(_dcm(1,2), _dcm(2,2));
		T theta = -asin(_dcm(0,2));
		T psi = atan2(_dcm(0,1), _dcm(0,0));
		return Vector3<T>(phi, theta, psi);
	}
};

}
