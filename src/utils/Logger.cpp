#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "ExternalAI/IAICallback.h"

#include "./Logger.hpp"
#include "./Util.hpp"
#include "../main/DFolders.hpp"

std::string Logger::GetLogName() const {
	if (init) {
		return name;
	}

	time_t t;
	time(&t);
	struct tm* lt = localtime(&t);

	std::stringstream ss;
		ss << std::string(AI_LOG_DIR);
		ss << "[";
		ss << cb->GetMapName();
		ss << "-";
		ss << cb->GetModName();
		ss << "][";
		ss << lt->tm_mon + 1;
		ss << "-";
		ss << lt->tm_mday;
		ss << "-";
		ss << lt->tm_year + 1900;
		ss << "][";
		ss << lt->tm_hour;
		ss << lt->tm_min;
		ss << "][team";
		ss << cb->GetMyTeam();
		ss << "]";

	std::string relName = ss.str();
	std::string absName = GetAbsFileName(cb, relName);
	std::cout << "Logging to " << absName << std::endl;
	return absName;
}

std::string Logger::GetInGameTime() {
	const int simFrame = cb->GetCurrentFrame();
	const int sec      = (simFrame / 30) % 60;
	const int min      = ((simFrame / 30) - sec) / 60;

	std::stringstream ss;
		ss << "[";
		ss << std::setw(2);
		ss << std::setfill('0');
		ss << std::setprecision(2);
		ss << min;
		ss << ":";
		ss << std::setw(2);
		ss << std::setfill('0');
		ss << std::setprecision(2);
		ss << sec;
		ss << "]";

	return ss.str();
}