#pragma once

#include <string>
#include <netdb.h>

namespace utils::network {

class AddressInfo {
public:
	/// @brief @ctor sets up the address info for a server
	/// @param ip_addr server IPV4 address
	/// @param port server port
	AddressInfo(const std::string& ip_addr, uint16_t port);
	virtual ~AddressInfo();

	addrinfo *get_ptr() const { return _ptr; }

private:
	addrinfo *_ptr;
};

}
