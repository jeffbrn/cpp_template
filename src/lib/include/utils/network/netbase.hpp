#pragma once

#include <cstdint>
#include <string>
#include "utils/log/ilogger.hpp"
#include "addr_info.hpp"

namespace utils::network {

/// @brief connects to a TCP server to send and receive data. msg format is 4 bytes for message length in bytes then the message in bytes
class NetworkBase {
protected:
	NetworkBase(log::ILogger *log);

	bool setup_client_socket(int domain, int type, int protocol);
	bool setup_client_socket(AddressInfo &addr_info);
	bool setup_server_socket(int type, uint16_t port);

	int get_skt_fd() const;

	void set_recv_timeout(int sec, int usec);

	void set_send_timeout(int sec, int usec);

	bool connect(AddressInfo &addr_info);

	void close_socket();

private:
	log::ILogger *_log;
	int _skt_fd {-1};
};

}
