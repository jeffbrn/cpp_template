#pragma once

#include "utils/log/ilogger.hpp"

namespace utils::log {

class NullLogger : public ILogger {
public:
	explicit NullLogger(LogLevel level) : ILogger(level) {}

protected:
	virtual void log(const char *, LogLevel) override {}
};

}
