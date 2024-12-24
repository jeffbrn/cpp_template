#pragma once

#include <cstdint>

namespace utils::network {

class NetworkBase {
protected:
	bool send_msg(int skt, const uint8_t *msg_buff, uint32_t msg_len);
	uint32_t recv_msg(int skt, uint8_t *rcv_buff, uint32_t buff_len);
};

}
