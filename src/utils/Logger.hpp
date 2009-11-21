#ifndef CARLAI_CLOGGER_HDR
#define CARLAI_CLOGGER_HDR

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
				log << "[BASIC] " << t; log << std::endl;
			} break;
			case LOG_DEBUG: {
				log << "[DEBUG] " << t; log << std::endl;
			} break;
			case LOG_ERROR: {
				log << "[ERROR] " << t; log << std::endl;
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

#define LOG_BASIC(l, m) { std::stringstream ss; ss << m; l->Log(ss.str(), utils::LOG_BASIC); }
#define LOG_DEBUG(l, m) { std::stringstream ss; ss << m; l->Log(ss.str(), utils::LOG_DEBUG); }
#define LOG_ERROR(l, m) { std::stringstream ss; ss << m; l->Log(ss.str(), utils::LOG_ERROR); }

#endif
