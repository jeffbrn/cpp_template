#include "utils/network/addr_info.hpp"
#include <stdexcept>

namespace utils::network {

AddressInfo::AddressInfo(const std::string& ip_addr, uint16_t port) {
	addrinfo hint {0};
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	auto rv = getaddrinfo(ip_addr.c_str(), std::to_string(port).c_str(), &hint, &_ptr);
	if (rv) {
		throw std::runtime_error("failed to get address info");
	}
}

AddressInfo::~AddressInfo() {
	freeaddrinfo(_ptr);
}

}
