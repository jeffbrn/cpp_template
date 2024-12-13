#pragma once

#include <string>
#include <cstdint>

namespace utils::network {

class TcpClient {
public:
	TcpClient(std::string ip_addr, uint16_t port);
	virtual ~TcpClient();

	size_t send(const uint8_t* send_msg, size_t msg_len, uint8_t* recv_msg, size_t max_rcv_msg_len);

private:
	int _skt_fd = 1;
};

}
