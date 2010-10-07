#include "LegacyCpp/IGlobalAICallback.h"
#include "LegacyCpp/IAICallback.h"

#include "./AIHelper.hpp"
#include "./AIDefines.hpp"

#include "../utils/Logger.hpp"
#include "../utils/Timer.hpp"
#include "../lua/AILuaModuleLoader.hpp"
#include "../units/AIUnitDefHandler.hpp"
#include "../groups/AIGroupHandler.hpp"
#include "../units/AIUnitHandler.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

pAIHelper AIHelper::activeInstance = NULL;

void AIHelper::Init(pIGlobalAICallback gcb, unsigned int i) {
	rcb  = gcb->GetAICallback();
	ccb  = gcb->GetCheatInterface();
	team = i;

	initFrame = rcb->GetCurrentFrame();
	currFrame = initFrame;

	aiLogger         = new AILogger(); aiLogger->OpenLog(aiLogger->GetLogName() + "[log].txt");
	aiTimer          = new AITimer(); aiTimer->OpenLog(aiLogger->GetLogName() + "[timings].txt");

	luaModuleLoader  = new LuaModuleLoader();
	aiUnitDefHandler = new AIUnitDefHandler();
	aiGroupHandler   = new AIGroupHandler();
	aiUnitHandler    = new AIUnitHandler();
	ecoState         = new EcoState();
	gameMap          = new GameMap();

	aiUnitDefHandler->OpenLog(aiLogger->GetLogName() + "[unitdefs].txt");
	aiUnitDefHandler->WriteLog();
	aiUnitDefHandler->CloseLog();

	ecoState->Init();
	gameMap->Init();

	initialized = true;
}

void AIHelper::Release() {
	aiLogger->CloseLog();

	aiTimer->SetLogFrame(rcb->GetCurrentFrame());
	aiTimer->WriteLog();
	aiTimer->CloseLog();

	delete aiLogger;
	delete aiTimer;

	delete luaModuleLoader;
	delete aiUnitDefHandler;
	delete aiGroupHandler;
	delete aiUnitHandler;
	delete ecoState;
	delete gameMap;
}
