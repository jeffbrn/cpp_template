#include "utils/network/tcp_client.hpp"

namespace utils::network {

TcpClient::TcpClient(std::string ip_addr, uint16_t port) {}

virtual TcpClient::~TcpClient() {}

size_t TcpClient::send(const uint8_t* send_msg, size_t msg_len, uint8_t* recv_msg, size_t max_rcv_msg_len) {}

}
