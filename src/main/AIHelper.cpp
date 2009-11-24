#include "ExternalAI/IGlobalAICallback.h"
#include "ExternalAI/IAICallback.h"

#include "./AIHelper.hpp"
#include "./DFolders.hpp"

#include "../utils/Logger.hpp"
#include "../utils/Timer.hpp"
#include "../globals/EcoState.hpp"
#include "../units/AIUnitDefHandler.hpp"

void AIHelper::Init(IGlobalAICallback* gcb, int i) {
	rcb  = gcb->GetAICallback();
	ccb  = gcb->GetCheatInterface();
	team = i;

	logger           = new Logger(rcb);
	timer            = new Timer();

	aiunitdefhandler = new AIUnitDefHandler(this);
	ecostate         = new EcoState();

	logger->OpenLog(logger->GetLogName() + "[log].txt");
	timer->OpenLog(logger->GetLogName() + "[timings].txt");
	aiunitdefhandler->OpenLog(logger->GetLogName() + "[unitdefs].txt");
}

void AIHelper::Release() {
	logger->CloseLog();

	timer->SetLogFrame(rcb->GetCurrentFrame());
	timer->WriteLog();
	timer->CloseLog();

	aiunitdefhandler->WriteLog();
	aiunitdefhandler->CloseLog();

	delete logger;
	delete timer;

	delete aiunitdefhandler;
	delete ecostate;
}
