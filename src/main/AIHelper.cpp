#include "ExternalAI/IGlobalAICallback.h"
#include "ExternalAI/IAICallback.h"

#include "./AIHelper.hpp"
#include "./DFolders.hpp"

#include "../utils/Logger.hpp"
#include "../utils/Timer.hpp"
#include "../lua/LuaModuleLoader.hpp"
#include "../units/AIUnitDefHandler.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

void AIHelper::Init(IGlobalAICallback* gcb, int i) {
	rcb  = gcb->GetAICallback();
	ccb  = gcb->GetCheatInterface();
	team = i;

	logger           = new Logger(rcb);
	timer            = new Timer();

	luaModuleLoader  = new LuaModuleLoader(this);

	aiUnitDefHandler = new AIUnitDefHandler(this);
	ecoState         = new EcoState();
	gameMap          = new GameMap();

	logger->OpenLog(logger->GetLogName() + "[log].txt");
	timer->OpenLog(logger->GetLogName() + "[timings].txt");
	aiUnitDefHandler->OpenLog(logger->GetLogName() + "[unitdefs].txt");

	ecoState->Init(this);
	gameMap->Init(this);
}

void AIHelper::Release() {
	logger->CloseLog();

	timer->SetLogFrame(rcb->GetCurrentFrame());
	timer->WriteLog();
	timer->CloseLog();

	aiUnitDefHandler->WriteLog();
	aiUnitDefHandler->CloseLog();

	delete logger;
	delete timer;

	delete luaModuleLoader;

	delete aiUnitDefHandler;
	delete ecoState;
	delete gameMap;
}
