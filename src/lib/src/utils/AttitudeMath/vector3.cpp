#include "utils/AttitudeMath/vector3.hpp"

namespace utils::AttitudeMath {

template<typename T>
Vector3<T>& Vector3<T>::operator+=(T val) {
	_elem[0] += val; _elem[1] += val; _elem[2] += val;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator-=(T val) {
	_elem[0] -= val; _elem[1] -= val; _elem[2] -= val;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(T val) {
	_elem[0] *= val; _elem[1] *= val; _elem[2] *= val;
	return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator/=(T val) {
	_elem[0] /= val; _elem[1] /= val; _elem[2] /= val;
	return *this;
}

}
