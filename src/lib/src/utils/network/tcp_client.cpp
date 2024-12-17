#include "utils/network/tcp_client.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

namespace utils::network {

TcpClient::TcpClient(std::string ip_addr, uint16_t port) {
	addrinfo hint {0}, *res;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	auto rv = getaddrinfo(ip_addr.c_str(), to_string(port).c_str(), &hint, &res);
	if (!rv) return;

	_skt_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_skt_fd <= 0) {
		freeaddrinfo(res);
		return;
	}

	struct timeval tv {1, 0};
	setsockopt(_skt_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));

	rv = connect(_skt_fd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	_valid = true;
}

TcpClient::~TcpClient() {
	if (_valid && close(_skt_fd) == -1) {
		perror("close");
	}
}

size_t TcpClient::send(const uint8_t* send_msg, size_t msg_len, uint8_t* recv_msg, size_t max_rcv_msg_len) {
	if (!_valid) return 0;
	if (send_msg != nullptr && msg_len > 0) {
		auto rv = ::send(_skt_fd, send_msg, msg_len, 0);
		if (rv < 0) {
			// error sending - now what
		}
	}
	if (recv_msg == nullptr) return 0;
	size_t bytes_read = 0, total_bytes = 0;
	auto buff = recv_msg;
	while ((bytes_read = recv(_skt_fd, buff, max_rcv_msg_len-total_bytes, 0)) > 0) {
		total_bytes += bytes_read;
		buff += bytes_read;
		if (total_bytes >= max_rcv_msg_len) break;
	}
	return total_bytes;
}

}
