#include <iostream>

#include "./AIMain.hpp"
#include "./AIHelper.hpp"
#include "./MAI.hpp"

#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../lua/AILuaCallBackHandler.hpp"
#include "../lua/AILuaModule.hpp"
#include "../groups/AIGroup.hpp"
#include "../utils/Factory.hpp"
#include "../units/AIUnit.hpp"
#include "../utils/Logger.hpp"

unsigned int AIMain::aiInstances = 0;

AIMain::AIMain(): aiInstance(0) {
}
AIMain::~AIMain() {
}



void AIMain::InitAI(pIGlobalAICallback gcb, int team) {
	std::cout << AI_CODENAME << std::endl;

	aiInstance = aiInstances++;
	aih = new AIHelper();
	aih->Init(gcb, team);

	LOG_BASIC(aih->logger, AI_CODENAME)
}

void AIMain::ReleaseAI() {
	aiInstances--;
	aih->Release();

	if (aiInstances == 0)
	{
		Factory<LuaModule>::Free();
		Factory<AIGroup>::Free();
		Factory<AIUnit>::Free();
	}
	delete aih;
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::UnitFinished(int unitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::UnitIdle(int unitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::UnitDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::EnemyDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::UnitMoveFailed(int unitID) {
	LuaCallBackHandler::SetHelper(aih);
}


void AIMain::EnemyEnterLOS(int enemyUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::EnemyLeaveLOS(int enemyUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::EnemyEnterRadar(int enemyUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::EnemyLeaveRadar(int enemyUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}

void AIMain::EnemyDestroyed(int enemyUnitID, int attackerUnitID) {
	LuaCallBackHandler::SetHelper(aih);
}


void AIMain::GotChatMsg(const char* msg, int playerNum) {
	LuaCallBackHandler::SetHelper(aih);
}

int AIMain::HandleEvent(int msgID, const void* msgData) {
	LuaCallBackHandler::SetHelper(aih);

	switch (msgID)
	{
		case AI_EVENT_UNITGIVEN:
		{
			const ChangeTeamEvent* cte = (const ChangeTeamEvent*) msgData;
			UnitFinished(cte->unit);
		} break;
		case AI_EVENT_UNITCAPTURED:
		{
			const ChangeTeamEvent* cte = (const ChangeTeamEvent*) msgData;
			UnitDestroyed(cte->unit, -1);
		} break;
	}

	return 0;
}


void AIMain::Update() {
	LuaCallBackHandler::SetHelper(aih);

	cUint32 currentFrame = aih->rcb->GetCurrentFrame();

	switch (currentFrame)
	{
		case 0: // update economic state
		{
			aih->ecoState->Update();
		} break;
		case 1: // update 
		{
		} break;
		case 2: // update
		{
		} break;
		case 3: // update
		{
		} break;
		case 4: // update
		{
		} break;
	}
}
