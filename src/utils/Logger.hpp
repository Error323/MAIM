#ifndef LOGGER_HDR
#define LOGGER_HDR

#include <string>
#include <fstream>
#include <sstream>

#include "./ILogger.hpp"

class IAICallback;

enum LogLevel {
	LOG_BASIC = 0,
	LOG_DEBUG = 1,
	LOG_ERROR = 2,
};

class Logger: public ILogger {
public:
	Logger(IAICallback* _cb): init(false), cb(_cb) {
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
			} break;
			case LOG_DEBUG: {
				log << "[DEBUG] " << t;
			} break;
			case LOG_ERROR: {
				log << "[ERROR] " << t;
			} break;
			default: {
			} break;
		}

		return *this;
	}

private:
	bool init;
	std::string name;
	IAICallback *cb;
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
