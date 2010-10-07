#ifndef AI_TIMER_HDR
#define AI_TIMER_HDR

#include <string>
#include <fstream>
#include <map>
#include <vector>

#include "./ILogger.hpp"


struct AITimer;
struct AIScopedTimer {
public:
	AIScopedTimer(const std::string&, AITimer*);
	~AIScopedTimer();

	struct AITimingDatum {
		std::string task;
		unsigned int t1;
		unsigned int t2;
		unsigned int t3;
		unsigned int d;   // timer depth
		unsigned int i;   // index into vtimings
	};

private:
	AITimer* t;
	AITimingDatum td;
};

struct AITimer: public ILogger {
public:
	AITimer(): timerDepth(0), timerTotalTime(0), timerTotalCalls(0) {
		vtimings.reserve(30 * 60 * 60);
	}

	unsigned int GetTaskTime(const std::string& t);
	void WriteLog();

	std::map<std::string, unsigned int> mtimings;
	std::map<std::string, unsigned int> mcounts;
	std::vector<AIScopedTimer::AITimingDatum> vtimings;

	unsigned int timerDepth;      // depth of the most recent ScopedTimer
	unsigned int timerTotalTime;  // total time taken by all timed tasks
	unsigned int timerTotalCalls; // total number of timers constructed
};

#endif
