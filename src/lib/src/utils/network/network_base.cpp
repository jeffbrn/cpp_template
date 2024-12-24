#include "utils/network/network_base.hpp"
#include <arpa/inet.h>
#include <iostream>

using namespace std;

namespace utils::network {

bool NetworkBase::send_msg(int skt, const uint8_t *msg_buff, uint32_t msg_len) {
	//send message length to server
	uint32_t hdr = htonl(msg_len);
	auto rv = ::send(skt, &hdr, sizeof(hdr), 0);
	if (rv != sizeof(hdr)) {
		//
cout << "*ERR* failed to send header" << endl;
		return false;
	}
	//send data in send buffer with message length in bytes first
	rv = ::send(skt, msg_buff, msg_len, 0);
	if (rv < 0) {
		// error sending - now what
cout << "*ERR* failed to send message" << endl;
		return false;
	}
cout << "RECV_MSG: sent " << rv << " bytes, total msg len = " << msg_len << endl;
	return true;
}

uint32_t NetworkBase::recv_msg(int skt, uint8_t *rcv_buff, uint32_t buff_len) {
	uint32_t hdr;
	uint32_t bytes_read = ::recv(skt, &hdr, sizeof(hdr), 0);
	if (bytes_read != sizeof(hdr)) {
cout << "*ERR* failed to recv header" << endl;
		return 0;
	}
	uint32_t expected_len = ntohl(hdr);
	if (expected_len > buff_len) {
cout << "*ERR* failed to recv - msg is too long" << endl;
		return 0;
	}
	uint32_t total_bytes = 0;
	while (total_bytes < expected_len) {
		if ((bytes_read = ::recv(skt, rcv_buff, buff_len-total_bytes, 0)) > 0) {
			total_bytes += bytes_read;
			rcv_buff += bytes_read;
		}
cout << "RECV_MSG: received " << bytes_read << " bytes, total read = " << total_bytes << ", expected len = " << expected_len << endl;
	}
	return total_bytes;
}

}
