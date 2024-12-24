#include <gtest/gtest.h>
#include "settings.hpp"
#include "utils/network/tcp_client.hpp"
#include "utils/network/tcp_listener.hpp"
#include "utils/log/console_logger.hpp"
#include "utils/log/null_logger.hpp"
#include <iostream>
#include <memory>

using namespace std;
using namespace utils::network;

namespace unit_tests::utils::network {

class NetworkTesting;
static NetworkTesting *TEST_INSTANCE;

class NetworkTesting : public ::testing::Test {
protected:
	unique_ptr<::utils::log::ILogger> _log {};

	void SetUp() override {
		#if SHOW_LOGS
		_log = make_unique<::utils::log::ConsoleLogger>(::utils::log::ILogger::LogLevel::DEBUG);
		#else
		_log = make_unique<::utils::log::NullLogger>(::utils::log::ILogger::LogLevel::DEBUG);
		#endif

		_test_port = 23673;
		TEST_INSTANCE = this;
	}

	void InitSendBuffer() {
		for(size_t i = 0; i < send_len; ++i) {
			auto v = static_cast<uint8_t>(i % 256);
			send_buff[i] = v;
		}
	}

public:
	uint16_t _test_port;
	static constexpr size_t MAX_LEN = 10*1024*1024;
	uint8_t send_buff[MAX_LEN] {0};
	uint8_t recv_buff[MAX_LEN] {0};
	size_t send_len {0}, recv_len {0};
};

std::pair<const uint8_t*, size_t> svr_handler(const uint8_t *buff, size_t buff_len) {
	cout << "*** in handler ***" << endl;
	EXPECT_EQ(TEST_INSTANCE->send_len, buff_len);
	for(int i = 0; i < buff_len; ++i) {
		EXPECT_EQ(TEST_INSTANCE->send_buff[i], buff[i]);
	}
	return make_pair(nullptr, 0);
}

TEST_F(NetworkTesting, ClientSendSmallData) {
	TcpListener svr(_log.get(), _test_port);
	svr.set_msg_handler(&svr_handler);
	this_thread::sleep_for(1000ms);
	TcpClient client(_log.get(), "127.0.0.1", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 10;
	InitSendBuffer();
	client.send(this->send_buff, this->send_len, nullptr, 0);
	this_thread::sleep_for(1000ms);
}

TEST_F(NetworkTesting, ClientSendMaxData) {
	TcpListener svr(_log.get(), _test_port);
	svr.set_msg_handler(&svr_handler);
	this_thread::sleep_for(1000ms);
	TcpClient client(_log.get(), "127.0.0.1", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 1000000;
	client.send(this->send_buff, this->send_len, nullptr, 0);
	this_thread::sleep_for(1000ms);
}

}
