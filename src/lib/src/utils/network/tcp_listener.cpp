#include "utils/network/tcp_listener.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace utils::network {

TcpListener::TcpListener(log::ILogger *log, uint16_t port, uint32_t buffer_size) : NetworkBase(log),
	_port(port), _buff_len(buffer_size), _buff(make_unique<uint8_t[]>(buffer_size))
{
	sockaddr_in svr_addr {};
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);
	_skt_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_skt_fd == -1) {
		_logger->error("SERVER: failed to create socket");
		throw runtime_error("failed to create socket");
	}

	if (bind(_skt_fd, reinterpret_cast<const sockaddr*>(&svr_addr), sizeof(svr_addr)) < 0) {
		_logger->error("SERVER: failed to create bind socket to port");
		throw runtime_error("failed to bind socket to port");
	}

	int flags = fcntl(_skt_fd, F_GETFL, 0);
	auto result = fcntl(_skt_fd, F_SETFL, flags | O_NONBLOCK);
	if (result) {
		_logger->error("SERVER: failed to set socket properties");
		throw runtime_error("failed to set socket properties");
	}

	_running = true;
	_thrd = std::move(thread(&TcpListener::handler, this));
}

TcpListener::~TcpListener() {
	// stop thread from accepting connections
	_running = false;
	if (_thrd.joinable()) {
		_thrd.join();
	}
	// close the socket
	close(_skt_fd);
}

void TcpListener::set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> msg_handler) {
	_msg_handler = std::move(msg_handler);
}

void TcpListener::handler() {
	using namespace std::chrono_literals;

	if (_skt_fd == -1) return;
	listen(_skt_fd, 0);
	while (_running) {
		sockaddr_in client_addr {};
		socklen_t addr_len = sizeof(client_addr);
		int client_skt = -1;
		// busy wait for incoming connection
		while ((client_skt = accept(_skt_fd, reinterpret_cast<sockaddr*>(&client_addr), &addr_len)) < 0) {
			this_thread::sleep_for(50ms);
			if (!_running) goto EXIT_THREAD; // signalled to stop
		}
		_logger->info("SERVER: received client connection");

		while (true) {
			_logger->debug("SERVER: waiting for client message");
			ssize_t bytes_read = NetworkBase::recv_msg(client_skt, _buff.get(), _buff_len);
			if (bytes_read < 0) {
				// client disconnected
				break;
			}
			if (_msg_handler) {
				//send message to the handler and get response if any
				auto [send_buff, send_len] = _msg_handler(_buff.get(), bytes_read);
				_logger->debug("SERVER: sending %d bytes", send_len);
				NetworkBase::send_msg(client_skt, send_buff, send_len);
			} else {
				_logger->warn("SERVER: no message handler set, sending empty response");
				NetworkBase::send_msg(client_skt, nullptr, 0);
			}
		}
		_logger->info("SERVER: client disconnected");
		close(client_skt);
	}

EXIT_THREAD:
;
}

}
