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
	/// @brief @ctor sets up a socket to listen for incoming messages
	/// @param log logger to use for class status messages
	/// @param port port to listen on for incoming messages
	/// @throw 
	TcpListener(log::ILogger *log, uint16_t port);
	virtual ~TcpListener();

	/// @brief sets the callback function to handle incoming messages
	/// @param pointer to the message hadnler
	void set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)>);

private:
	int _skt_fd {-1};
	uint16_t _port;
	std::thread _thrd;
	std::atomic<bool> _running {false}; // flag to stop the thread
	static constexpr int BUFF_SZ = 1000000; //65535;
	uint8_t _buff[BUFF_SZ] {};

	/// @brief this is the thread function that listens for incoming messages
	void handler();
	std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> _msg_handler {};
};

}
