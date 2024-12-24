#pragma once

#include <cstdint>
#include <stdexcept>
#include "utils/log/ilogger.hpp"

namespace utils::network {

class NetworkBase {
protected:
	NetworkBase(log::ILogger *log) : _logger(log) {
		if (_logger == nullptr) {
			throw std::invalid_argument("logger cannot be null");
		}
	};

	bool send_msg(int skt, const uint8_t *msg_buff, uint32_t msg_len);
	uint32_t recv_msg(int skt, uint8_t *rcv_buff, uint32_t buff_len);

	log::ILogger *_logger;
};

}
