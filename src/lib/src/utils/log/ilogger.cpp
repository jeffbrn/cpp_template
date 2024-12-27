#include "utils/log/ilogger.hpp"
#include <cstdio>

namespace utils::log {

using namespace std;

void ILogger::error(const char *msg, ...) {
	if (_filter >= LogLevel::ERROR) {
		va_list args;
		va_start(args, msg);
		{
			lock_guard lock(_mtx);
			auto txt = get_text(msg, args);
			va_end(args);
			log(txt, LogLevel::ERROR);
		}
	}
}

void ILogger::warn(const char *msg, ...) {
	if (_filter >= LogLevel::WARNING) {
		va_list args;
		va_start(args, msg);
		{
			lock_guard lock(_mtx);
			auto txt = get_text(msg, args);
			va_end(args);
			log(txt, LogLevel::WARNING);
		}
	}
}

void ILogger::info(const char *msg, ...) {
	if (_filter >= LogLevel::INFO) {
		va_list args;
		va_start(args, msg);
		{
			lock_guard lock(_mtx);
			auto txt = get_text(msg, args);
			va_end(args);
			log(txt, LogLevel::INFO);
		}
	}
}

void ILogger::debug(const char *msg, ...) {
	if (_filter >= LogLevel::DEBUG) {
		va_list args;
		va_start(args, msg);
		{
			lock_guard lock(_mtx);
			auto txt = get_text(msg, args);
			va_end(args);
			log(txt, LogLevel::DEBUG);
		}
	}
}

const char* ILogger::get_text(const char *msg , va_list args) {
	_msg_buff[0] = '\0';
	vsnprintf(_msg_buff, MAX_MSG_LEN, msg, args);
	return _msg_buff;
}

}
