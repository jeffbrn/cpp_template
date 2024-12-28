#pragma once

#include "utils/log/ilogger.hpp"

namespace utils::log {

class ConsoleLogger : public ILogger {
public:
	explicit ConsoleLogger(LogLevel level) : ILogger(level) {}

protected:
	virtual void log(const char *, LogLevel) override;
};

}
