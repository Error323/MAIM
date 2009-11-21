#include <SDL/SDL_timer.h>

#include <map>
#include <vector>
#include <fstream>

#include "./Timer.hpp"

ScopedTimer::ScopedTimer(const std::string& task, Timer* timer): t(timer) {
	t->timerDepth += 1;

	td.task = task;
	td.t1   = SDL_GetTicks();
	td.t2   = td.t1;
	td.t3   = 0;
	td.d    = t->timerDepth - 1;
	td.i    = t->vtimings.size();

	t->vtimings.push_back(td);
}

ScopedTimer::~ScopedTimer() {
	td.t2 = SDL_GetTicks();
	td.t3 = td.t2 - td.t1;

	t->timerDepth      -= 1;
	t->timerTotalTime  += td.t3;
	t->timerTotalCalls += 1;

	// <task> took <t3> millisecs, add to its total
	if (t->mtimings.find(td.task) != t->mtimings.end()) {
		t->mtimings[td.task] += td.t3;
	} else {
		t->mtimings[td.task] = td.t3;
	}

	// update how often we've timed this task
	if (t->mcounts.find(td.task) != t->mcounts.end()) {
		t->mcounts[td.task] += 1;
	} else {
		t->mcounts[td.task] = 1;
	}

	t->vtimings[td.i] = td;
}



unsigned int Timer::GetTaskTime(const std::string& t) {
	if (mtimings.find(t) != mtimings.end()) {
		return mtimings[t];
	}
	return 0;
}

void Timer::WriteLog() {
	/*
	// too much output
	for (std::vector<ScopedTimer::STimingDatum>::const_iterator it = vtimings.begin(); it != vtimings.end(); it++) {
		const ScopedTimer::STimingDatum& td = *it;

		for (unsigned int n = 0; n < td.d; n++) {
			log << "  ";
		}

		log << td.task << " (" << td.t3 << "ms)" << std::endl;
		log.flush();
	}
	*/

	log << "absolute (ms), average (ms), and relative (%) timings per task:" << std::endl;
	log.flush();

	for (std::map<std::string, unsigned int>::const_iterator it = mtimings.begin(); it != mtimings.end(); it++) {
		const std::string& taskName = it->first;
		const unsigned int taskTime = it->second; // cumulative

		log << "\t";
		log << taskName << "\t";
		log << taskTime << "\t";
		log <<  (taskTime / double(logFrame)) << "\t";
		log << ((taskTime / double(timerTotalTime)) * 100.0f) << "%";
		log << std::endl;
		log.flush();
	}

	log << std::endl;
	log << "absolute and relative number of calls per task:" << std::endl;
	log.flush();

	for (std::map<std::string, unsigned int>::const_iterator it = mcounts.begin(); it != mcounts.end(); it++) {
		const std::string& taskName  = it->first;
		const unsigned int taskCalls = it->second;

		log << "\t";
		log << taskName << "\t";
		log << taskCalls << "\t";
		log << ((taskCalls / double(timerTotalCalls)) * 100.0f) << "%";
		log << std::endl;
		log.flush();
	}
}
