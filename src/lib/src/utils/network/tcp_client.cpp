#include "utils/network/tcp_client.hpp"

using namespace std;

namespace utils::network {

TcpClient::TcpClient(log::ILogger *log, std::string ip_addr, uint16_t port) : TcpBase(log), NetworkBase(log) {
	_logger->debug("CLIENT: connecting to %s:%d", ip_addr.c_str(), port);
	// convert server address to an IPV4 address
	AddressInfo svr_addr(ip_addr, port);
	if (!setup_client_socket(svr_addr)) {
		_logger->error("CLIENT: failed to setup socket");
		return;
	}
	// set a 1 second receive timeout
	set_recv_timeout(1, 0);
	if (!connect(svr_addr)) {
		_logger->error("CLIENT: failed to connect to server");
		close_socket();
		return;
	}
	_logger->info("CLIENT: connected to %s:%d", ip_addr.c_str(), port);
	_valid = true;
}

TcpClient::~TcpClient() {
	close_socket();
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
	if (!TcpBase::send_msg(get_skt_fd(), send_msg, msg_len)) {
		_logger->error("CLIENT: failed to send message");
		return make_pair(false, 0);
	}
	_logger->debug("CLIENT: sent %d bytes", msg_len);
	_logger->debug("CLIENT: receiving response");
	uint32_t recvd_bytes = 0;
	if ((recvd_bytes = TcpBase::recv_msg(get_skt_fd(), recv_msg, max_rcv_msg_len)) < 0) {
		_logger->error("CLIENT: failed to receive response");
		return make_pair(false, 0);
	}
	_logger->debug("CLIENT: received %d bytes", recvd_bytes);
	return make_pair(true, recvd_bytes);
}

}
