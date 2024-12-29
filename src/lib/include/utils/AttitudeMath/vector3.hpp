#pragma once

#include <array>

namespace utils::AttitudeMath {

template<typename T>
class Vector3 {
public:
	Vector3() : _elem{0} {}
	Vector3(T x, T y, T z) : _elem{x,y,z} {}

	// Accessors
	T x() const { return _elem[0]; }
	T y() const { return _elem[1]; }
	T z() const { return _elem[2]; }
	T operator[](int i) const { return _elem[i]; }
	T& operator[](int i) { return _elem[i]; }
	void x(T val) { _elem[0] = val; }
	void y(T val) { _elem[1] = val; }
	void z(T val) { _elem[2] = val; }

	// Operator assignments - scalar
	Vector3<T>& operator+=(T val);
	Vector3<T>& operator-=(T val);
	Vector3<T>& operator*=(T val);
	Vector3<T>& operator/=(T val);
	
private:
	std::array<T,3> _elem;
};

}