#ifndef ILOGGER
#define ILOGGER

#include <fstream>

// logs to a file
class ILogger {
public:
	virtual ~ILogger() {}
	virtual void OpenLog(const std::string&);
	virtual void CloseLog();
	virtual void WriteLog() {}
	virtual void SetLogFrame(unsigned int f) { logFrame = f; }

protected:
	std::ofstream log;
	unsigned int logFrame;
};

#endif
