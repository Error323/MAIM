#include "./ILogger.hpp"

void ILogger::OpenLog(const std::string& n) {
	log.open(n.c_str(), std::ios::out);
}
void ILogger::CloseLog() {
	log.flush();
	log.close();
}
