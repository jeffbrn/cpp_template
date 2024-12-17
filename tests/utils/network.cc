#include <gtest/gtest.h>
#include "utils/network/tcp_client.hpp"
#include "utils/network/tcp_listener.hpp"
#include <iostream>

using namespace std;
using namespace utils::network;

namespace unit_tests::utils::network {

class NetworkTesting : public ::testing::Test {
protected:
	void SetUp() override {
		_test_port = 23673;
	}

	uint16_t _test_port;
};

std::pair<const uint8_t*, size_t> svr_handler(const uint8_t *buff, size_t buff_len) {
	cout << "*** in handler ***" << endl;
	return make_pair(nullptr, 0);
}

TEST_F(NetworkTesting, ListenerSetup) {
	TcpListener svr(_test_port);
	svr.set_msg_handler(&svr_handler);
	this_thread::sleep_for(1000ms);
	TcpClient client("127.0.0.1", _test_port);
	EXPECT_TRUE(client.is_valid());
	constexpr size_t send_len = 10;
	uint8_t send_buff[send_len];
	client.send(send_buff, send_len, nullptr, 0);
	this_thread::sleep_for(1000ms);
}

}
