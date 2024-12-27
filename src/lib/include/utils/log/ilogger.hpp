#pragma once

#include <cstdarg>
#include <mutex>

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

	/// @brief interface ctor
	/// @param level minimum log level to be logged
	explicit ILogger(LogLevel level) : _filter(level) {}

	/// @brief log an error message
	/// @param  message format 
	/// @param  message arguments
	virtual void error(const char *, ...);
	/// @brief log a warning message
	/// @param  message format 
	/// @param  message arguments
	virtual void warn(const char *, ...);
	/// @brief log an informational message
	/// @param  message format 
	/// @param  message arguments
	virtual void info(const char *, ...);
	/// @brief log a debugging message
	/// @param  message format 
	/// @param  message arguments
	virtual void debug(const char *, ...);

	LogLevel get_filter() const { return _filter; }

protected:
	/// @brief convert the message format and arguments to a string
	/// @param  message format
	/// @param  message arguments
	/// @return pointer to the message string
	virtual const char* get_text(const char *, va_list);
	
	/// @brief log the formatted message
	/// @param pointer to the message string
	/// @param log level of the message
	virtual void log(const char *, LogLevel) = 0;

private:
	LogLevel _filter;
	static constexpr int MAX_MSG_LEN = 1024;
	char _msg_buff[MAX_MSG_LEN];
	std::mutex _mtx;
};

}
