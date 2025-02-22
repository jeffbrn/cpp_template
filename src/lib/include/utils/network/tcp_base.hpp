#pragma once

#include <cstdint>
#include <mutex>
#include <stdexcept>
#include "utils/log/ilogger.hpp"

namespace utils::network {

class TcpBase {
protected:
	TcpBase(log::ILogger *log) : _logger(log) {
		if (_logger == nullptr) {
			throw std::invalid_argument("logger cannot be null");
		}
	};

	/// @brief send message with header that contains the message length and then the message itself
	/// @param skt socket to send the message
	/// @param msg_buff pointer to the message buffer
	/// @param msg_len length of the message
	/// @return true if successful, false otherwise
	bool send_msg(int skt, const uint8_t *msg_buff, uint32_t msg_len);

	/// @brief receive message with header that contains the message length and then the message itself
	/// @param skt socker to receive the message
	/// @param rcv_buff pointer to the buffer where the message will be stored
	/// @param buff_len length of the buffer
	/// @return length of the received message or -1 if failed
	ssize_t recv_msg(int skt, uint8_t *rcv_buff, uint32_t buff_len);

	log::ILogger *_logger;

private:
	std::mutex _mtx;
};

}
