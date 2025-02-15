#include <gtest/gtest.h>
#include "utils/serialization/test_data_poco.hpp"

using namespace std;
using namespace utils::serialization;

namespace unit_tests::utils::serialization {

class SerializationTesting : public ::testing::Test {
protected:

	void SetUp() override {
	}

};

TEST_F(SerializationTesting, SerializeTestData) {
	uint8_t buff[1024] {0};
	TestDataPoco initial {3, {1.1,2.2,3.3}};
	auto len = POCO_SERIALIZE(initial, buff, sizeof(buff));
	EXPECT_GT(len, 0);
	auto result = POCO_DESERIALIZE(TestDataPoco, buff, len);
	EXPECT_EQ(initial.itema, result.itema);
	EXPECT_EQ(initial.itemb, result.itemb);
}

}
