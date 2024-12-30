#pragma once

#include <array>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

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
	Vector3<T>& operator+=(const T rhs) {
		_elem[0] += rhs; _elem[1] += rhs; _elem[2] += rhs;
		return *this;
	}
	Vector3<T>& operator-=(const T rhs) {
		_elem[0] -= rhs; _elem[1] -= rhs; _elem[2] -= rhs;
		return *this;
	}
	Vector3<T>& operator*=(const T rhs) {
		_elem[0] *= rhs; _elem[1] *= rhs; _elem[2] *= rhs;
		return *this;
	}
	Vector3<T>& operator/=(const T rhs) {
		if (rhs == 0) {
			throw std::invalid_argument("Cannot divide by zero");
		}
		_elem[0] /= rhs; _elem[1] /= rhs; _elem[2] /= rhs;
		return *this;
	}

	// Operator assignments - vector
	Vector3<T>& operator+=(const Vector3<T>& rhs) {
		_elem[0] += rhs[0]; _elem[1] += rhs[1]; _elem[2] += rhs[2];
		return *this;
	}
	Vector3<T>& operator-=(const Vector3<T>& rhs) {
		_elem[0] -= rhs[0]; _elem[1] -= rhs[1]; _elem[2] -= rhs[2];
		return *this;
	}
	Vector3<T>& operator*=(const Vector3<T>& rhs) {
		_elem[0] *= rhs[0]; _elem[1] *= rhs[1]; _elem[2] *= rhs[2];
		return *this;
	}
	Vector3<T>& operator/=(const Vector3<T>& rhs) {
		if (rhs.x() == 0 || rhs.y() == 0 || rhs.z() == 0) {
			throw std::invalid_argument("Cannot divide by zero");
		}
		_elem[0] /= rhs[0]; _elem[1] /= rhs[1]; _elem[2] /= rhs[2];
		return *this;
	}

	// Vector operations
	T norm() const;
	void normalize();
	Vector3<T> unit() const;
	Vector3<T> cross(const Vector3<T>& rhs) const {
		T x = (this->y() * rhs.z()) - (this->z() * rhs.y());
		T y = (this->z() * rhs.x()) - (this->x() * rhs.z());
		T z = (this->x() * rhs.y()) - (this->y() * rhs.x());
		return Vector3<T>(x, y, z);
	}
	T dot(const Vector3<T>& rhs) const { return x()*rhs.x() + y()*rhs.y() + z()*rhs.z(); }

	// Predefined vectors
	static const Vector3<T> xAxis() { return Vector3<T>(1,0,0); }
	static const Vector3<T> yAxis() { return Vector3<T>(0,1,0); }
	static const Vector3<T> zAxis() { return Vector3<T>(0,0,1); }

private:
	std::array<T,3> _elem;
};

// Equality
template<typename T>
static bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

// Vector / Vector Elementwise Operations
template<typename T>
static Vector3<T> operator-(const Vector3<T>& rhs) {
	return Vector3<T>(-rhs.x(), -rhs.y(), -rhs.z());
}
template<typename T>
static Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}
template<typename T>
static Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}
template<typename T>
static Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}
template<typename T>
static Vector3<T> operator/(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	if (rhs.x() == 0 || rhs.y() == 0 || rhs.z() == 0) {
		throw std::invalid_argument("Cannot divide by a vector with a zero dimension");
	}
	return Vector3<T>(lhs.x() / rhs.x(), lhs.y() / rhs.y(), lhs.z() / rhs.z());
}

// Vector / Scalar Operations
template<typename T>
static Vector3<T> operator+(const Vector3<T>& lhs, const T rhs) {
	return Vector3<T>(lhs.x() + rhs, lhs.y() + rhs, lhs.z() + rhs);
}
template<typename T>
static Vector3<T> operator-(const Vector3<T>& lhs, const T rhs) {
	return Vector3<T>(lhs.x() - rhs, lhs.y() - rhs, lhs.z() - rhs);
}
template<typename T>
static Vector3<T> operator*(const Vector3<T>& lhs, const T rhs) {
	return Vector3<T>(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs);
}
template<typename T>
static Vector3<T> operator/(const Vector3<T>& lhs, const T rhs) {
	if (rhs == 0) {
		throw std::invalid_argument("Cannot divide by zero");
	}
	return Vector3<T>(lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs);
}

template<typename T>
static Vector3<T> operator+(const T lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs + rhs.x(), lhs + rhs.y(), lhs + rhs.z());
}
template<typename T>
static Vector3<T> operator-(const T lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs - rhs.x(), lhs - rhs.y(), lhs - rhs.z());
}
template<typename T>
static Vector3<T> operator*(const T lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}
template<typename T>
static Vector3<T> operator/(const T lhs, const Vector3<T>& rhs) {
	if (rhs.x() == 0 || rhs.y() == 0 || rhs.z() == 0) {
		throw std::invalid_argument("Cannot divide by zero");
	}
	return Vector3<T>(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z());
}

// stream operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
	os << "[ " << v.x() << "," << v.y() << "," << v.z() << " ]";
	return os;
}

}
