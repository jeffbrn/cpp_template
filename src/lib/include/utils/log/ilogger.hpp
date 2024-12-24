#pragma once

#include <cstdarg>

namespace utils::log {

class ILogger {
public:
	enum class LogLevel {
		NONE,
		ERROR,
		WARNING,
		INFO,
		DEBUG,
	};

	explicit ILogger(LogLevel level) : _filter(level) {}

	virtual void error(const char *, ...);
	virtual void warn(const char *, ...);
	virtual void info(const char *, ...);
	virtual void debug(const char *, ...);

	LogLevel get_filter() const { return _filter; }

protected:
	virtual const char* get_text(const char *, va_list);
	virtual void log(const char *, LogLevel) = 0;

private:
	LogLevel _filter;
	static constexpr int MAX_MSG_LEN = 1024;
	char _msg_buff[MAX_MSG_LEN];
};

}
