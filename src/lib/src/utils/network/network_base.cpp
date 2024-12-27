#include "utils/network/network_base.hpp"
#include <arpa/inet.h>

using namespace std;

namespace utils::network {

bool NetworkBase::send_msg(int skt, const uint8_t *msg_buff, uint32_t msg_len) {
	// make sure only a single thread is sending at a time
	lock_guard lock(_mtx);
	// send message header - the length of the message in bytes
	uint32_t hdr = htonl(msg_len);
	auto rv = ::send(skt, &hdr, sizeof(hdr), 0);
	if (rv != sizeof(hdr)) {
		// didn't send the whole header
		_logger->error("failed to send header");
		return false;
	}

	uint32_t send_len = 0;
	while (send_len < msg_len) {
		//send data in send buffer with message length in bytes first
		rv = ::send(skt, msg_buff+send_len, msg_len-send_len, 0);
		if (rv < 0) {
			// error sending packet
			_logger->error("failed to send message");
			return false;
		}
		send_len += rv;
		_logger->debug("sent %d bytes, total msg len = %d", rv, msg_len);
		// loop until all bytes are sent
	}
	return true;
}

uint32_t NetworkBase::recv_msg(int skt, uint8_t *rcv_buff, uint32_t buff_len) {
	// make sure only a single thread is receiving at a time
	lock_guard lock(_mtx);
	uint32_t hdr;
	uint32_t bytes_read = ::recv(skt, &hdr, sizeof(hdr), 0);
	if (bytes_read != sizeof(hdr)) {
		// didn't receive the whole header or recieve timed-out
		_logger->error("failed to recv header");
		return 0;
	}
	uint32_t expected_len = ntohl(hdr);
	if (expected_len > buff_len) {
		// message is too long for buffer
		_logger->error("failed to recv - msg is too long");
		return 0;
	}
	uint32_t total_bytes = 0;
	while (total_bytes < expected_len) {
		if ((bytes_read = ::recv(skt, rcv_buff, buff_len-total_bytes, 0)) > 0) {
			total_bytes += bytes_read;
			rcv_buff += bytes_read;
		}
		_logger->debug("RECV_MSG: received %d bytes, total read = %d, expected len = %d", bytes_read, total_bytes, expected_len);
		// loop until all bytes are received
	}
	return total_bytes;
}

}
