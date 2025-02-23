#include "utils/network/tcp_listener.hpp"
#include <unistd.h>

using namespace std;

namespace utils::network {

TcpListener::TcpListener(log::ILogger *log, uint16_t port, uint32_t buffer_size) : TcpBase(log), NetworkBase(log),
	_port(port), _buff_len(buffer_size), _buff(make_unique<uint8_t[]>(buffer_size))
{
	if (!setup_server_socket(SOCK_STREAM, port)) {
		throw runtime_error("failed to setup socket");
	}
	_running = true;
	_thrd = std::move(thread(&TcpListener::handler, this));
}

TcpListener::~TcpListener() {
	// stop thread from accepting connections
	_running = false;
	if (_thrd.joinable()) {
		_thrd.join();
	}
	// close the socket
	close_socket();
}

void TcpListener::set_msg_handler(std::function<std::pair<const uint8_t*, size_t>(const uint8_t*, size_t)> msg_handler) {
	_msg_handler = std::move(msg_handler);
}

bool TcpListener::is_listening() const {
	return _listening;
}

void TcpListener::wait_for_listening() {
	unique_lock<std::mutex> lck(_mtx);
	_logger->debug("SERVER: waiting for listening");
	_cv.wait(lck, [this] () { return _listening; });
	_logger->debug("SERVER: is up and running");
}

void TcpListener::handler() {
	using namespace std::chrono_literals;

	if (get_skt_fd() == -1) return;
	listen(get_skt_fd(), 0);
	_logger->info("SERVER: ready to accept connections, signalling waiters");
	{
		std::unique_lock<std::mutex> lck(_mtx);
		_listening = true;
	}
	_cv.notify_all();
	while (_running) {
		sockaddr_in client_addr {};
		socklen_t addr_len = sizeof(client_addr);
		int client_skt = -1;
		// busy wait for incoming connection
		while ((client_skt = accept(get_skt_fd(), reinterpret_cast<sockaddr*>(&client_addr), &addr_len)) < 0) {
			this_thread::sleep_for(50ms);
			if (!_running) goto EXIT_THREAD; // signalled to stop
		}
		_logger->info("SERVER: received client connection");

		while (true) {
			_logger->debug("SERVER: waiting for client message");
			ssize_t bytes_read = TcpBase::recv_msg(client_skt, _buff.get(), _buff_len);
			if (bytes_read < 0) {
				// client disconnected
				break;
			}
			if (_msg_handler) {
				//send message to the handler and get response if any
				auto [send_buff, send_len] = _msg_handler(_buff.get(), bytes_read);
				_logger->debug("SERVER: sending %d bytes", send_len);
				TcpBase::send_msg(client_skt, send_buff, send_len);
			} else {
				_logger->warn("SERVER: no message handler set, sending empty response");
				TcpBase::send_msg(client_skt, nullptr, 0);
			}
		}
		_logger->info("SERVER: client disconnected");
		close(client_skt);
	}

EXIT_THREAD:
;
}

}
