#include "./HAIInterface.hpp"
#include "./AIHelper.hpp"
#include "./DFolders.hpp"

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

	logger           = new Logger();
	timer            = new Timer();

	luaModuleLoader  = new LuaModuleLoader();

	aiUnitDefHandler = new AIUnitDefHandler();
	aiGroupHandler   = new AIGroupHandler();
	aiUnitHandler    = new AIUnitHandler();
	ecoState         = new EcoState();
	gameMap          = new GameMap();

	logger->OpenLog(logger->GetLogName() + "[log].txt");
	timer->OpenLog(logger->GetLogName() + "[timings].txt");
	aiUnitDefHandler->OpenLog(logger->GetLogName() + "[unitdefs].txt");

	ecoState->Init();
	gameMap->Init();

	initialized = true;
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
	delete aiGroupHandler;
	delete aiUnitHandler;
	delete ecoState;
	delete gameMap;
}
