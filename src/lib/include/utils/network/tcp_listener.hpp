#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <utility>
#include <thread>
#include <atomic>
#include "network_base.hpp"

namespace utils::network {

class TcpListener : public NetworkBase {
public:
	TcpListener(log::ILogger *log, uint16_t port);
	virtual ~TcpListener();

	void set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)>);
	void wait();

private:
	bool _valid {false};
	int _skt_fd {-1};
	uint16_t _port;
	std::thread _thrd;
	std::atomic<bool> _running {false};
	static constexpr int BUFF_SZ = 1000000; //65535;
	uint8_t _buff[BUFF_SZ] {};

	void handler();
	std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> _msg_handler {};
};

}
