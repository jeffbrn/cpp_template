#pragma once

#include <string>
#include <utility>
#include <unistd.h>
#include "tcp_base.hpp"
#include "addr_info.hpp"

namespace utils::network {

/// @brief connects to a TCP server to send and receive data. msg format is 4 bytes for message length in bytes then the message in bytes
class NetworkBase {
protected:
	NetworkBase(log::ILogger *log) : _log(log) {
		if (_log == nullptr) {
			throw std::invalid_argument("logger cannot be null");
		}
	};

	bool setup_socket(int domain, int type, int protocol) {
		_skt_fd = socket(domain, type, protocol);
		if (_skt_fd <= 0) {
			_log->error("failed to create socket");
			return false;
		}
		return true;
	}
	bool setup_socket(AddressInfo &addr_info) {
		return setup_socket(addr_info.get_ptr()->ai_family, addr_info.get_ptr()->ai_socktype, addr_info.get_ptr()->ai_protocol);
	}

	int get_skt_fd() const {
		return _skt_fd;
	}

	void set_recv_timeout(int sec, int usec) {
		struct timeval tv {sec, usec};
		setsockopt(_skt_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
	}

	void set_send_timeout(int sec, int usec) {
		struct timeval tv {sec, usec};
		setsockopt(_skt_fd, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
	}

	bool connect(AddressInfo &addr_info) {
		if (_skt_fd <= 0) {
			_log->error("socket not setup");
			return false;
		}
		auto res = addr_info.get_ptr();
		auto rv = ::connect(_skt_fd, res->ai_addr, res->ai_addrlen);
		if (rv) {
			_log->error("failed to connect to server");
			return false;
		}
		return true;
	}

	void close_socket() {
		if (_skt_fd > 0) {
			close(_skt_fd);
			_skt_fd = -1;
		}
	}

private:
	log::ILogger *_log;
	int _skt_fd {-1};
};

}
