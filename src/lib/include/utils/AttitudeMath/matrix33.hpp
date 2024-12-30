#pragma once

#include <array>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include "utils/AttitudeMath/vector3.hpp"

namespace utils::AttitudeMath {

template<typename T>
class Matrix33 {
public:
	Matrix33() : _elem({0}) {}
	Matrix33(std::initializer_list<T> l) {
		if (l.size() != 9) throw std::invalid_argument("invalid initializer list size");
		std::copy(l.begin(), l.end(), _elem.begin());
	}
	Matrix33(const Vector3<T>& col1, const Vector3<T>& col2, const Vector3<T>& col3) {
		_elem[0] = col1.x(); _elem[1] = col2.x(); _elem[2] = col3.x();
		_elem[3] = col1.y(); _elem[4] = col2.y(); _elem[5] = col3.y();
		_elem[6] = col1.z(); _elem[7] = col2.z(); _elem[8] = col3.z();
	}
	Matrix33(const Vector3<T>& diag) : Matrix33() {
		_elem[0] = diag.x(); _elem[4] = diag.y(); _elem[8] = diag.z();
	}
	Matrix33(const Matrix33<T>& m) : _elem(m._elem) {}

	// Accessors
	T operator()(int i, int j) const { return get(i, j); }
	T& operator()(int i, int j) { return get(i, j); }
	const T* const data() const { return _elem.data(); }
	Vector3<T> diag() const { return Vector3<T>(_elem[0], _elem[4], _elem[8]); }

	// Operator assignments - scalar
	Matrix33<T>& operator+=(const T rhs) {
		for(int i = 0; i < 9; i++) _elem[i] += rhs;
		return *this;
	}
	Matrix33<T>& operator-=(const T rhs) {
		for(int i = 0; i < 9; i++) _elem[i] -= rhs;
		return *this;
	}
	Matrix33<T>& operator*=(const T rhs) {
		for(int i = 0; i < 9; i++) _elem[i] *= rhs;
		return *this;
	}
	Matrix33<T>& operator/=(const T rhs) {
		if (rhs == 0) throw std::invalid_argument("Cannot divide by zero");
		for(int i = 0; i < 9; i++) _elem[i] /= rhs;
		return *this;
	}
	// Operator assignments - matrix
	Matrix33<T>& operator+=(const Matrix33<T>& rhs) {
		for(int i = 0; i < 9; i++) _elem[i] += rhs._elem[i];
		return *this;
	}
	Matrix33<T>& operator-=(const Matrix33<T>& rhs) {
		for(int i = 0; i < 9; i++) _elem[i] -= rhs._elem[i];
		return *this;
	}

	// Matrix Operations
	Matrix33<T>& transpose() {
		std::swap(_elem[1], _elem[3]);
		std::swap(_elem[2], _elem[6]);
		std::swap(_elem[5], _elem[7]);
		return *this;
	}
	T determinant() const {
		return _elem[0]*(_elem[4]*_elem[8] - _elem[5]*_elem[7]) -
			_elem[1]*(_elem[3]*_elem[8] - _elem[5]*_elem[6]) +
			_elem[2]*(_elem[3]*_elem[7] - _elem[4]*_elem[6]);
	}
	Matrix33<T>& inverse();

	
	// Predefined matrices
	static Matrix33<T> Identity() { return Matrix33<T> {1,0,0,0,1,0,0,0,1}; }

private:
	// row based element layout
	std::array<T, 9> _elem;

	T get(int i, int j) const {
		if (i < 0 || i > 2) throw std::invalid_argument("invalid row index");
		if (j < 0 || j > 2) throw std::invalid_argument("invalid column index");
		return _elem[i * 3 + j];
	}
	T& get(int i, int j) {
		if (i < 0 || i > 2) throw std::invalid_argument("invalid row index");
		if (j < 0 || j > 2) throw std::invalid_argument("invalid column index");
		return _elem[i * 3 + j];
	}
};

// Equality
template<typename T>
static bool operator==(const Matrix33<T>& lhs, const Matrix33<T>& rhs) {
	for(int i = 0; i < 9; i++) {
		if (lhs.data()[i] != rhs.data()[i]) return false;
	}
	return true;
}

// Matrix / Matrix Elementwise Operations
template<typename T>
static Matrix33<T> operator-(const Matrix33<T>& rhs) {
	Matrix33<T> retval(rhs);
	return retval *= -1;
}
template<typename T>
static Matrix33<T> operator+(const Matrix33<T>& lhs, const Matrix33<T>& rhs) {
	return Matrix33<T>(lhs) += rhs;
}
template<typename T>
static Matrix33<T> operator-(const Matrix33<T>& lhs, const Matrix33<T>& rhs) {
	return Matrix33<T>(lhs) -= rhs;
}
template<typename T>
static Matrix33<T> operator*(const Matrix33<T>& lhs, const Matrix33<T>& rhs) {
	std::array<T, 9> data {0};
	return Matrix33<T>({
		lhs.data()[0]*rhs.data()[0] + lhs.data()[1]*rhs.data()[3] + lhs.data()[2]*rhs.data()[6],
		lhs.data()[0]*rhs.data()[1] + lhs.data()[1]*rhs.data()[4] + lhs.data()[2]*rhs.data()[7],
		lhs.data()[0]*rhs.data()[2] + lhs.data()[1]*rhs.data()[5] + lhs.data()[2]*rhs.data()[8],
		lhs.data()[3]*rhs.data()[0] + lhs.data()[4]*rhs.data()[3] + lhs.data()[5]*rhs.data()[6],
		lhs.data()[3]*rhs.data()[1] + lhs.data()[4]*rhs.data()[4] + lhs.data()[5]*rhs.data()[7],
		lhs.data()[3]*rhs.data()[2] + lhs.data()[4]*rhs.data()[5] + lhs.data()[5]*rhs.data()[8],
		lhs.data()[6]*rhs.data()[0] + lhs.data()[7]*rhs.data()[3] + lhs.data()[8]*rhs.data()[6],
		lhs.data()[6]*rhs.data()[1] + lhs.data()[7]*rhs.data()[4] + lhs.data()[8]*rhs.data()[7],
		lhs.data()[6]*rhs.data()[2] + lhs.data()[7]*rhs.data()[5] + lhs.data()[8]*rhs.data()[8]
	});
}
template<typename T>
static Vector3<T> operator*(const Matrix33<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(
		lhs.data()[0]*rhs.x() + lhs.data()[1]*rhs.y() + lhs.data()[2]*rhs.z(),
		lhs.data()[3]*rhs.x() + lhs.data()[4]*rhs.y() + lhs.data()[5]*rhs.z(),
		lhs.data()[6]*rhs.x() + lhs.data()[7]*rhs.y() + lhs.data()[8]*rhs.z()
	);
}

// Matrix / Scalar Operations
template<typename T>
static Matrix33<T> operator+(const Matrix33<T>& lhs, const T rhs) {
	Matrix33<T> retval(lhs);
	return retval += rhs;
}
template<typename T>
static Matrix33<T> operator-(const Matrix33<T>& lhs, const T rhs) {
	Matrix33<T> retval(lhs);
	return retval -= rhs;
}
template<typename T>
static Matrix33<T> operator*(const Matrix33<T>& lhs, const T rhs) {
	Matrix33<T> retval(lhs);
	return retval *= rhs;
}
template<typename T>
static Matrix33<T> operator/(const Matrix33<T>& lhs, const T rhs) {
	if (rhs == 0) throw std::invalid_argument("Cannot divide by zero");
	Matrix33<T> retval(lhs);
	return retval /= rhs;
}
template<typename T>
static Matrix33<T> operator+(const T lhs, const Matrix33<T>& rhs) {
	Matrix33<T> retval(rhs);
	return retval += lhs;
}
template<typename T>
static Matrix33<T> operator-(const T lhs, const Matrix33<T>& rhs) {
	Matrix33<T> retval {
		lhs-rhs.data()[0], lhs-rhs.data()[1], lhs-rhs.data()[2],
		lhs-rhs.data()[3], lhs-rhs.data()[4], lhs-rhs.data()[5],
		lhs-rhs.data()[6], lhs-rhs.data()[7], lhs-rhs.data()[8]
	};
	return retval;
}
template<typename T>
static Matrix33<T> operator*(const T lhs, const Matrix33<T>& rhs) {
	Matrix33<T> retval(rhs);
	return retval *= lhs;

}
template<typename T>
static Matrix33<T> operator/(const T lhs, const Matrix33<T>& rhs) {
	for(int i = 0; i < 9; i++) {
		if (rhs.data()[i] == 0) throw std::invalid_argument("Cannot divide by a matrix with a zero element");
	}
	Matrix33<T> retval {
		lhs/rhs.data()[0], lhs/rhs.data()[1], lhs/rhs.data()[2],
		lhs/rhs.data()[3], lhs/rhs.data()[4], lhs/rhs.data()[5],
		lhs/rhs.data()[6], lhs/rhs.data()[7], lhs/rhs.data()[8]
	};
	return retval;
}

// Matrix Operations
template<typename T>
static Matrix33<T> transpose(const Matrix33<T>& lhs) {
	Matrix33<T> retval {
		lhs.data()[0], lhs.data()[3], lhs.data()[6],
		lhs.data()[1], lhs.data()[4], lhs.data()[7],
		lhs.data()[2], lhs.data()[5], lhs.data()[8]
	};
	return retval;
}
template<typename T>
static Matrix33<T> inverse(const Matrix33<T>& lhs) {
	Matrix33<T> retval(lhs);
	return retval.inverse();
}

// stream operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix33<T>& m) {
	os << "[ " << m(0,0) << "," << m(0,1) << "," << m(0,2) << "; ";
	os << m(1,0) << "," << m(1,1) << "," << m(1,2) << "; ";
	os << m(2,0) << "," << m(2,1) << "," << m(2,2) << " ]";
	return os;
}

}
