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

std::pair<const uint8_t*, size_t> svr_handler_only_rcv(const uint8_t *buff, size_t buff_len) {
	EXPECT_EQ(TEST_INSTANCE->send_len, buff_len);
	for(int i = 0; i < buff_len; ++i) {
		EXPECT_EQ(TEST_INSTANCE->send_buff[i], buff[i]);
	}
	return make_pair(nullptr, 0);
}

std::pair<const uint8_t*, size_t> svr_handler_only_snd(const uint8_t *buff, size_t buff_len) {
	EXPECT_EQ(buff_len, 0);
	return make_pair(TEST_INSTANCE->send_buff, TEST_INSTANCE->send_len);
}

std::pair<const uint8_t*, size_t> svr_handler_send_rcv(const uint8_t *buff, size_t buff_len) {
	EXPECT_EQ(TEST_INSTANCE->send_len, buff_len);
	for(int i = 0; i < buff_len; ++i) {
		EXPECT_EQ(TEST_INSTANCE->send_buff[i], buff[i]);
		if (buff[i] == 255) {
			TEST_INSTANCE->send_buff[i] = 0;
		} else {
			TEST_INSTANCE->send_buff[i] = buff[i] + 1;
		}
	}
	return make_pair(TEST_INSTANCE->send_buff, TEST_INSTANCE->send_len);
}

TEST_F(NetworkTesting, ListenerStartupShutdown) {
	TcpListener svr(_log.get(), _test_port);
	this_thread::sleep_for(1000ms);
}

TEST_F(NetworkTesting, ClientSendSmallData) {
	TcpListener svr(_log.get(), _test_port);
	svr.set_msg_handler(&svr_handler_only_rcv);
	this_thread::sleep_for(10ms);
	TcpClient client(_log.get(), "localhost", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 10;
	InitSendBuffer();
	auto [success, recv_len] = client.send(this->send_buff, this->send_len, nullptr, 0);
	EXPECT_TRUE(success);
	EXPECT_EQ(0, recv_len);
	this_thread::sleep_for(100ms);
}

TEST_F(NetworkTesting, ClientSendMaxData) {
	uint32_t buff_len = 4*1024*1024;
	TcpListener svr(_log.get(), _test_port, buff_len);
	svr.set_msg_handler(&svr_handler_only_rcv);
	this_thread::sleep_for(10ms);
	TcpClient client(_log.get(), "127.0.0.1", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = buff_len;
	InitSendBuffer();
	auto [success, recv_len] = client.send(this->send_buff, this->send_len, nullptr, 0);
	EXPECT_TRUE(success);
	EXPECT_EQ(0, recv_len);
	this_thread::sleep_for(100ms);
}

TEST_F(NetworkTesting, ClientOnlyRecvData) {
	TcpListener svr(_log.get(), _test_port);
	svr.set_msg_handler(&svr_handler_only_snd);
	this_thread::sleep_for(10ms);
	TcpClient client(_log.get(), "localhost", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 10;
	InitSendBuffer();
	auto [success, recv_len] = client.send(nullptr, 0, this->recv_buff, this->send_len);
	EXPECT_TRUE(success);
	EXPECT_EQ(this->send_len, recv_len);
	this_thread::sleep_for(100ms);
}

TEST_F(NetworkTesting, ClientSendAndRecvData) {
	TcpListener svr(_log.get(), _test_port);
	svr.set_msg_handler(&svr_handler_send_rcv);
	this_thread::sleep_for(10ms);
	TcpClient client(_log.get(), "localhost", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 10;
	InitSendBuffer();
	this->recv_len = 10;
	auto [success, recv_len] = client.send(this->send_buff, this->send_len, this->recv_buff, this->send_len);
	EXPECT_TRUE(success);
	EXPECT_EQ(this->send_len, recv_len);
	for(int i = 0; i < recv_len; ++i) {
		EXPECT_EQ(send_buff[i], this->recv_buff[i]);
	}
	this_thread::sleep_for(100ms);
}

TEST_F(NetworkTesting, ListenPortinUse) {
	TcpListener svr1(_log.get(), _test_port);
	this_thread::sleep_for(10ms);
	EXPECT_THROW(TcpListener svr2(_log.get(), _test_port), std::runtime_error);
	this_thread::sleep_for(10ms);
}

TEST_F(NetworkTesting, ClientSvrNotListening) {
	TcpClient client(_log.get(), "127.0.0.1", _test_port);
	EXPECT_FALSE(client.is_valid());
}

TEST_F(NetworkTesting, ClientBadAddress) {
	TcpClient client(_log.get(), "555.0.0.1", _test_port);
	EXPECT_FALSE(client.is_valid());
}

TEST_F(NetworkTesting, ClientBadSendBufferLen) {
	TcpListener svr(_log.get(), _test_port);
	TcpClient client(_log.get(), "localhost", _test_port);
	EXPECT_TRUE(client.is_valid());
	auto [success, recv_len] = client.send(nullptr, 10, nullptr, 0);
	EXPECT_FALSE(success);
	EXPECT_EQ(0, recv_len);
}

TEST_F(NetworkTesting, ClientBadRecvBufferLen) {
	TcpListener svr(_log.get(), _test_port);
	TcpClient client(_log.get(), "localhost", _test_port);
	EXPECT_TRUE(client.is_valid());
	this->send_len = 10;
	InitSendBuffer();
	auto [success, recv_len] = client.send(this->send_buff, this->send_len, nullptr, 10);
	EXPECT_FALSE(success);
	EXPECT_EQ(0, recv_len);
	this_thread::sleep_for(100ms);
}

}
