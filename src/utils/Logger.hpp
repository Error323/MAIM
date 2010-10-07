#ifndef AI_LOGGER_HDR
#define AI_LOGGER_HDR

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

class AILogger: public ILogger {
public:
	AILogger(): init(false) {
		name = GetLogName();
		init = true;
	}
	~AILogger() {
	}

	std::string GetLogName() const;
	std::string GetInGameTime();

	template<typename T> AILogger& Log(const T& t, LogLevel lvl = LOG_BASIC) {
		log << GetInGameTime() << " ";

		switch (lvl) {
			case LOG_BASIC: {
				log << "[BASIC] " << t;

				#ifdef DEBUG
				std::cout << "\E[0m[BASIC] " << t << "\E[0m";
				#endif
			} break;
			case LOG_DEBUG: {
				log << "[DEBUG] " << t;

				#ifdef DEBUG
				std::cout << "\E[0m\E[33m[DEBUG] " << t << "\E[0m";
				#endif
			} break;
			case LOG_ERROR: {
				log << "[ERROR] " << t;

				#ifdef DEBUG
				std::cout << "\E[0m\E[31m[ERROR] " << t << "\E[0m";
				#endif
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
