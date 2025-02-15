#pragma once

#include <cstdint>
#include <array>
#include "iserialization.hpp"

namespace utils::serialization {

struct TestDataPoco {
	int32_t itema;
	std::array<double, 3> itemb;

	std::size_t serialize(uint8_t *buffer, std::size_t buffer_len) const;
	static TestDataPoco deserialize(const uint8_t *buffer, std::size_t buffer_len);
};

}
