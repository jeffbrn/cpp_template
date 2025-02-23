#pragma once

#include <functional>
#include <utility>
#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>
#include "tcp_base.hpp"
#include "netbase.hpp"

namespace utils::network {

class TcpListener : public TcpBase, public NetworkBase {
public:
	/// @brief @ctor sets up a socket to listen for incoming messages
	/// @param log logger to use for class status messages
	/// @param port port to listen on for incoming messages
	/// @throw 
	TcpListener(log::ILogger *log, uint16_t port, uint32_t buffer_size = 1*1024*1024);
	virtual ~TcpListener();

	/// @brief sets the callback function to handle incoming messages
	/// @param pointer to the message hadnler
	void set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)>);
	bool is_listening() const;
	void wait_for_listening();

private:
	uint16_t _port;
	std::thread _thrd;
	std::atomic<bool> _running {false}; // flag to stop the thread
	uint32_t _buff_len {0};
	std::unique_ptr<uint8_t[]> _buff;
	// support to indicate when the listener is ready to receive messages
	bool _listening {false};
	std::mutex _mtx;
	std::condition_variable _cv;

	/// @brief this is the thread function that listens for incoming messages
	void handler();
	std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> _msg_handler {};
};

}
