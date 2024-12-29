#pragma once

#include <array>
#include <initializer_list>
#include <stdexcept>

namespace utils::AttitudeMath {

template<typename T>
class Vector3 {
public:
	Vector3() : _elem{0} {}
	Vector3(T x, T y, T z) : _elem{x,y,z} {}
	Vector3(std::initializer_list<T> list) {
		if (list.size() != 3) {
			throw std::invalid_argument("Vector3 initializer list must have 3 elements");
		}
		std::copy(list.begin(), list.end(), _elem.begin());
	}

	// Accessors
	T x() const { return _elem[0]; }
	T y() const { return _elem[1]; }
	T z() const { return _elem[2]; }
	T operator[](int i) const { if (i < 0 || i > 2) throw std::invalid_argument("invalid index"); return _elem[i]; }
	T& operator[](int i) { if (i < 0 || i > 2) throw std::invalid_argument("invalid index"); return _elem[i]; }
	void x(T val) { _elem[0] = val; }
	void y(T val) { _elem[1] = val; }
	void z(T val) { _elem[2] = val; }

	// Operator assignments - scalar
	Vector3<T>& operator+=(const T val) {
		_elem[0] += val; _elem[1] += val; _elem[2] += val;
		return *this;
	}
	Vector3<T>& operator-=(const T val) {
		_elem[0] -= val; _elem[1] -= val; _elem[2] -= val;
		return *this;
	}
	Vector3<T>& operator*=(const T val) {
		_elem[0] *= val; _elem[1] *= val; _elem[2] *= val;
		return *this;
	}
	Vector3<T>& operator/=(const T val) {
		_elem[0] /= val; _elem[1] /= val; _elem[2] /= val;
		return *this;
	}

	// Operator assignments - vector
	Vector3<T>& operator+=(const Vector3<T>& other) {
		_elem[0] += other[0]; _elem[1] += other[1]; _elem[2] += other[2];
		return *this;
	}
	Vector3<T>& operator-=(const Vector3<T>& other) {
		_elem[0] -= other[0]; _elem[1] -= other[1]; _elem[2] -= other[2];
		return *this;
	}
	Vector3<T>& operator*=(const Vector3<T>& other) {
		_elem[0] *= other[0]; _elem[1] *= other[1]; _elem[2] *= other[2];
		return *this;
	}
	Vector3<T>& operator/=(const Vector3<T>& other) {
		_elem[0] /= other[0]; _elem[1] /= other[1]; _elem[2] /= other[2];
		return *this;
	}

private:
	std::array<T,3> _elem;
};

// Equality
template<typename T>
static bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}
	

}