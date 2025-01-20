#pragma once

#include <iostream>
#include "utils/AttitudeMath/vector3.hpp"
#include "utils/AttitudeMath/matrix33.hpp"

namespace utils::AttitudeMath {

/// @brief Allows gogletest to print vector value
template<typename T>
void PrintTo(const Vector3<T>& v, std::ostream* os) {
	*os << "[ " << v.x() << "," << v.y() << "," << v.z() << " ]";
}

/// @brief Allows gogletest to print vector value
template<typename T>
void PrintTo(const Matrix33<T>& m, std::ostream* os) {
	*os << std::endl << "[ "
		<< m(0,0) << "," << m(0,1) << "," << m(0,2) << ";" << std::endl << "  "
		<< m(1,0) << "," << m(1,1) << "," << m(1,2) << ";" << std::endl << "  "
		<< m(2,0) << "," << m(2,1) << "," << m(2,2) << ";" << std::endl
		<< " ]";
}

}

namespace unit_tests::utils::AttitudeMath {

template<typename T>
void Test_NearEqual(const ::utils::AttitudeMath::Matrix33<T>& lhs, const ::utils::AttitudeMath::Matrix33<T>& rhs, T diff = std::numeric_limits<T>::epsilon()) {
	for(int i = 0; i < 9; i++) {
		EXPECT_NEAR(lhs.data()[i], rhs.data()[i], diff) << "Element " << i << " not equal";
	}
}

template<typename T>
void Test_NearEqual(const ::utils::AttitudeMath::Vector3<T>& lhs, const ::utils::AttitudeMath::Vector3<T>& rhs, T diff = std::numeric_limits<T>::epsilon()) {
	EXPECT_NEAR(lhs.x(), rhs.x(), diff) << "Element x not equal";
	EXPECT_NEAR(lhs.y(), rhs.y(), diff) << "Element y not equal";
	EXPECT_NEAR(lhs.z(), rhs.z(), diff) << "Element z not equal";
}

}
