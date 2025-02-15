#pragma once

#include <cstdint>
#include <string>
#include <map>

namespace utils::serialization {

#define POCO_SERIALIZE(obj, buffer, buffer_size) (obj).serialize((buffer), (buffer_size))

#define POCO_DESERIALIZE(type, buffer, buffer_size) type::deserialize((buffer), (buffer_size))

}
