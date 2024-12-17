#include "utils/network/tcp_listener.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace utils::network {

TcpListener::TcpListener(uint16_t port) {
	sockaddr_in svr_addr {};
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);
	_skt_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_skt_fd == -1) {
		cout << "ERR: failed to create server socket" << endl;
		return;
	}

	if (bind(_skt_fd, reinterpret_cast<const sockaddr*>(&svr_addr), sizeof(svr_addr)) < 0) {
		cout << "ERR: failed to create bind socket to port" << endl;
		return;
	}

	int flags = fcntl(_skt_fd, F_GETFL, 0);
	auto result = fcntl(_skt_fd, F_SETFL, flags | O_NONBLOCK);
	if (result) {
		cout << "ERR: failed to set socket properties" << endl;
		return;
	}

	_running = true;
	_thrd = std::move(thread(&TcpListener::handler, this));
	_valid = true;
	this_thread::sleep_for(1ms);
}

TcpListener::~TcpListener() {
	if (_valid) {
		wait();
		close(_skt_fd);
	}
}

void TcpListener::set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> msg_handler) {
	_msg_handler = std::move(msg_handler);
}

void TcpListener::wait() {
	if (_valid) {
		_running = false;
		if (_thrd.joinable()) {
			_thrd.join();
		}
	}
}

void TcpListener::handler() {
	using namespace std::chrono_literals;
	_running = true;
	if (_skt_fd == -1) return;
	listen(_skt_fd, 0);
	while (_running) {
		sockaddr_in client_addr {};
		socklen_t addr_len = sizeof(client_addr);
		int client_skt = -1;
		while ((client_skt = accept(_skt_fd, reinterpret_cast<sockaddr*>(&client_addr), &addr_len)) < 0) {
			this_thread::sleep_for(50ms);
			if (!_running) goto EXIT_THREAD;
		}
cout << "received client connection" << endl;

		ssize_t bytes_read;
		while ((bytes_read = recv(client_skt, _buff, BUFF_SZ, 0)) > 0) {
cout << "server received " << bytes_read << " bytes" << endl;
			if (_msg_handler) {
				auto [send_buff, send_len] = _msg_handler(_buff, bytes_read);
				if (send_buff && send_len > 0) {
cout << "sending " << send_len << " bytes" << endl;
					send(client_skt, send_buff, send_len, 0);
				}
			}
		}
		close(client_skt);
		if (bytes_read < 0) {
			//error
		}
	}

EXIT_THREAD:
;
}

}
