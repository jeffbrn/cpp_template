#include "utils/network/tcp_client.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace utils::network {

TcpClient::TcpClient(log::ILogger *log, std::string ip_addr, uint16_t port) : NetworkBase(log) {
	_logger->debug("CLIENT: connecting to %s:%d", ip_addr.c_str(), port);
	// convert server address to an IPV4 address
	addrinfo hint {0}, *res;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	auto rv = getaddrinfo(ip_addr.c_str(), to_string(port).c_str(), &hint, &res);
	if (rv) {
		_logger->error("CLIENT: failed to get address info, rv = %d", rv);
		return;
	}

	// instantiate client socket
	_skt_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_skt_fd <= 0) {
		_logger->error("CLIENT: failed to create socket");
		freeaddrinfo(res);
		return;
	}

	// set a 1 second receive timeout
	struct timeval tv {1, 0};
	setsockopt(_skt_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));

	// try to connect to server
	rv = connect(_skt_fd, res->ai_addr, res->ai_addrlen);
	if (rv) {
		_logger->error("CLIENT: failed to connect to server");
		close(_skt_fd);
		_skt_fd = -1;
		return;
	}
	freeaddrinfo(res);
	_logger->info("CLIENT: connected to %s:%d", ip_addr.c_str(), port);
	_valid = true;
}

TcpClient::~TcpClient() {
	if (_valid && close(_skt_fd) == -1) {
		perror("close");
	}
}

pair<bool, uint32_t> TcpClient::send(const uint8_t* send_msg, uint32_t msg_len, uint8_t* recv_msg, uint32_t max_rcv_msg_len) {
	if (!_valid) { //client isn't connected
		return make_pair(false, 0);
	}
	if (send_msg == nullptr && msg_len > 0) {
		_logger->error("CLIENT: Invalid send buffer length");
		return make_pair(false, 0);
	}
	if (recv_msg == nullptr && max_rcv_msg_len > 0) {
		_logger->error("CLIENT: Invalid receive buffer length");
		return make_pair(false, 0);
	}
	_logger->debug("CLIENT: sending request message");
	if (!NetworkBase::send_msg(_skt_fd, send_msg, msg_len)) {
		_logger->error("CLIENT: failed to send message");
		return make_pair(false, 0);
	}
	_logger->debug("CLIENT: sent %d bytes", msg_len);
	_logger->debug("CLIENT: receiving response");
	uint32_t recvd_bytes = 0;
	if ((recvd_bytes = NetworkBase::recv_msg(_skt_fd, recv_msg, max_rcv_msg_len)) < 0) {
		_logger->error("CLIENT: failed to receive response");
		return make_pair(false, 0);
	}
	_logger->debug("CLIENT: received %d bytes", recvd_bytes);
	return make_pair(true, recvd_bytes);
}

}
