#pragma once

#include <string>
#include <utility>
#include "network_base.hpp"

namespace utils::network {

/// @brief connects to a TCP server to send and receive data. msg format is 4 bytes for message length in bytes then the message in bytes
class TcpClient : public NetworkBase {
public:
	/// @brief @ctor sets up a TCP connection to the server to send and receive messages
	/// @param ip_addr server IPV4 address
	/// @param port server port
	TcpClient(std::string ip_addr, uint16_t port);
	virtual ~TcpClient();

	/// @brief sends a byteserialized message across the connection
	/// @param send_msg pointer to the send buffer, if null if no message is sent
	/// @param msg_len length of the send message in bytes
	/// @param recv_msg pointer to the receive buffer, if null if no return message is expected
	/// @param max_rcv_msg_len length of the receive buffer in bytes
	/// @return true if comms was successful, # bytes received from server in response
	std::pair<bool, uint32_t> send(const uint8_t* send_msg, uint32_t msg_len, uint8_t* recv_msg, uint32_t max_rcv_msg_len);

	/// @brief checks to see if there is a connection to the server
	/// @return true if connected and able to send & receive
	bool is_valid() const { return _valid; }

private:
	bool _valid {false};
	int _skt_fd {-1};
};

}
