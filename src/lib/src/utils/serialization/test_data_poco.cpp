#include "utils/serialization/test_data_poco.hpp"
#include "messages/test_generated.h"
#include <stdexcept>

namespace utils::serialization {

using namespace std;
using namespace flatbuffers;

std::size_t TestDataPoco::serialize(uint8_t *buffer, std::size_t buffer_len) const {
	flatbuffers::FlatBufferBuilder builder(256);
	Vec3 v (make_span(itemb));
	auto p = CreateTestData(builder, itema, &v);
	builder.Finish(p);
	std::size_t size = builder.GetSize();
	if (size > buffer_len) {
		return 0;
	}
	std::memcpy(buffer, builder.GetBufferPointer(), size);
	return size;
}

TestDataPoco TestDataPoco::deserialize(const uint8_t *buffer, std::size_t buffer_len) {
	auto v = Verifier(buffer, buffer_len);
	if (!VerifyTestDataBuffer(v)) {
		throw runtime_error("Invalid buffer");
	}
	auto p = GetTestData(buffer);
	if (p == nullptr) {
		throw runtime_error("Unable to deserialize");
	}
	auto sp = p->itemb()->p();
	TestDataPoco result {
		p->itema(),
		{{(*sp)[0], (*sp)[1], (*sp)[2]}}
	};
	return result;
}

}
