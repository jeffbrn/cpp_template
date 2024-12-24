#include "utils/log/console_logger.hpp"
#include <iostream>

namespace utils::log {

using namespace std;

void ConsoleLogger::log(const char *txt, LogLevel lvl) {
	switch (lvl) {
		case LogLevel::DEBUG:
			std::cout << "[DEBUG]: " << txt << std::endl;
			break;
		case LogLevel::INFO:
			std::cout << "[INFO ]: " << txt << std::endl;
			break;
		case LogLevel::WARNING:
			std::cout << "[WARN ]: " << txt << std::endl;
			break;
		case LogLevel::ERROR:
			std::cerr << "[ERROR]: " << txt << std::endl;
			break;
		default:
			break;
	}
}

}
