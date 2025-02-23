#include "utils/network/netbase.hpp"
#include <unistd.h>
#include <fcntl.h>

namespace utils::network {

NetworkBase::NetworkBase(log::ILogger *log) : _log(log) {
	if (_log == nullptr) {
		throw std::invalid_argument("logger cannot be null");
	}
};

bool NetworkBase::setup_client_socket(int domain, int type, int protocol) {
	_skt_fd = socket(domain, type, protocol);
	if (_skt_fd <= 0) {
		_log->error("CLIENT: failed to create socket");
		return false;
	}
	return true;
}
bool NetworkBase::setup_client_socket(AddressInfo &addr_info) {
	return setup_client_socket(addr_info.get_ptr()->ai_family, addr_info.get_ptr()->ai_socktype, addr_info.get_ptr()->ai_protocol);
}
bool NetworkBase::setup_server_socket(int type, uint16_t port) {
	sockaddr_in svr_addr {0};
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);
	_skt_fd = socket(AF_INET, type, 0);
	if (_skt_fd <= 0) {
		_log->error("SERVER:: failed to create socket");
		return false;
	}
	if (bind(_skt_fd, reinterpret_cast<const sockaddr*>(&svr_addr), sizeof(svr_addr)) < 0) {
		_log->error("SERVER: failed to create bind socket to port");
		return false;
	}

	// set the socket to non-blocking
	int flags = fcntl(_skt_fd, F_GETFL, 0);
	auto result = fcntl(_skt_fd, F_SETFL, flags | O_NONBLOCK);
	if (result) {
		_log->error("SERVER: failed to set socket properties");
		return false;
	}
	return true;
}

int NetworkBase::get_skt_fd() const {
	return _skt_fd;
}

void NetworkBase::set_recv_timeout(int sec, int usec) {
	struct timeval tv {sec, usec};
	setsockopt(_skt_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
}

void NetworkBase::set_send_timeout(int sec, int usec) {
	struct timeval tv {sec, usec};
	setsockopt(_skt_fd, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));
}

bool NetworkBase::connect(AddressInfo &addr_info) {
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

void NetworkBase::close_socket() {
	if (_skt_fd > 0) {
		close(_skt_fd);
		_skt_fd = -1;
	}
}

}
