#ifndef LOGGER_HDR
#define LOGGER_HDR

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./ILogger.hpp"

enum LogLevel {
	LOG_BASIC = 0,
	LOG_DEBUG = 1,
	LOG_ERROR = 2,
};

class Logger: public ILogger {
public:
	Logger(): init(false) {
		name = GetLogName();
		init = true;
	}
	~Logger() {
	}

	std::string GetLogName() const;
	std::string GetInGameTime();

	template<typename T> Logger& Log(const T& t, LogLevel lvl = LOG_BASIC) {
		log << GetInGameTime() << " ";

		switch (lvl) {
			case LOG_BASIC: {
				log << "[BASIC] " << t;
				std::cout << "\E[0m[BASIC] " << t << "\E[0m";
			} break;
			case LOG_DEBUG: {
				log << "[DEBUG] " << t;
				std::cout << "\E[0m\E[33m[DEBUG] " << t << "\E[0m";
			} break;
			case LOG_ERROR: {
				log << "[ERROR] " << t;
				std::cout << "\E[0m\E[31m[ERROR] " << t << "\E[0m";
			} break;
			default: {
			} break;
		}

		return *this;
	}

private:
	bool init;
	std::string name;
};

#define LOG_BASIC(MSG)                                                    \
do {                                                                      \
	std::stringstream ss;                                                 \
	ss << MSG;                                                            \
	AIHelper::GetActiveInstance()->GetLogger()->Log(ss.str(), LOG_BASIC); \
} while(0)

#define LOG_DEBUG(MSG)                                                    \
do {                                                                      \
	std::stringstream ss;                                                 \
	ss << MSG;                                                            \
	AIHelper::GetActiveInstance()->GetLogger()->Log(ss.str(), LOG_DEBUG); \
} while(0)

#define LOG_ERROR(MSG)                                                    \
do {                                                                      \
	std::stringstream ss;                                                 \
	ss << MSG;                                                            \
	AIHelper::GetActiveInstance()->GetLogger()->Log(ss.str(), LOG_ERROR); \
} while(0)

#endif
