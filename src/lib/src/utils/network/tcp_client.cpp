#include "utils/network/tcp_client.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace utils::network {

TcpClient::TcpClient(std::string ip_addr, uint16_t port) {
	// convert server address to an IPV4 address
	addrinfo hint {0}, *res;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	auto rv = getaddrinfo(ip_addr.c_str(), to_string(port).c_str(), &hint, &res);
	if (rv) {
		cout << "CLIENT: *ERR* failed to get address info, rv = " << rv << endl;
		return;
	}

	// instantiate client socker
	_skt_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_skt_fd <= 0) {
		cout << "CLIENT: *ERR* failed to create socket" << endl;
		freeaddrinfo(res);
		return;
	}

	// set a 1 second receive timeout
	struct timeval tv {1, 0};
	setsockopt(_skt_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv));

	// try to connect to server
	rv = connect(_skt_fd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
cout << "CLIENT: is connected" << endl;
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
	if (send_msg != nullptr && msg_len > 0) {
cout << "CLIENT: sending message" << endl;
		if (!NetworkBase::send_msg(_skt_fd, send_msg, msg_len)) {
cout << "CLIENT: *ERR* failed to send message" << endl;
			_valid = false;
			return make_pair(false, 0);
		}
cout << "CLIENT: sent " << msg_len << " bytes" << endl;
	}
	if (recv_msg == nullptr || max_rcv_msg_len == 0) {
		return make_pair(true, 0);
	}
cout << "CLIENT: receiving response" << endl;
uint32_t recvd_bytes = 0;
	if ((recvd_bytes = NetworkBase::recv_msg(_skt_fd, recv_msg, max_rcv_msg_len)) == 0) {
cout << "CLIENT: *ERR* failed to receive response" << endl;
		_valid = false;
		return make_pair(false, 0);
	}
cout << "CLIENT: received " << recvd_bytes << " bytes" << endl;
	return make_pair(true, recvd_bytes);
}

}
