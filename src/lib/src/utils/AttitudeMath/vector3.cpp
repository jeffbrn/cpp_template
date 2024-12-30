#include "utils/AttitudeMath/vector3.hpp"
#include <cmath>

using namespace std;

namespace utils::AttitudeMath {

template<typename T>
double do_norm(T x, T y, T z) {
	return sqrt(x * x + y * y + z * z);
}

template<>
int Vector3<int>::norm() const { return do_norm(x(), y(), z()); }
template<>
float Vector3<float>::norm() const { return do_norm(x(), y(), z()); }
template<>
double Vector3<double>::norm() const { return do_norm(x(), y(), z()); }

template<>
void Vector3<float>::normalize() {
	auto n = norm();
	if (n == 0.f) {
		throw std::invalid_argument("Cannot normalize a zero vector");
	}
	*this /= n;
}
template<>
void Vector3<double>::normalize() {
	auto n = norm();
	if (n == 0.0) {
		throw std::invalid_argument("Cannot normalize a zero vector");
	}
	*this /= norm();
}

template<>
Vector3<float> Vector3<float>::unit() const {
	auto n = norm();
	if (n == 0.f) {
		throw std::invalid_argument("Cannot get unit of a zero vector");
	}
	return *this / norm();
}
template<>
Vector3<double> Vector3<double>::unit() const {
	auto n = norm();
	if (n == 0.f) {
		throw std::invalid_argument("Cannot get unit of a zero vector");
	}
	return *this / norm();
}

}
