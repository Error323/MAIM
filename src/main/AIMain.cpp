#include <iostream>

#include "./AIMain.hpp"
#include "./AIHelper.hpp"
#include "./MAI.hpp"

#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../lua/AILuaCallBackHandler.hpp"
#include "../lua/AILuaModule.hpp"
#include "../groups/AIGroup.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitManager.hpp"
#include "../utils/Factory.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Debugger.hpp"

unsigned int AIMain::aiInstances = 0;

AIMain::AIMain(): aiInstance(0) {
	aih = new AIHelper();
}
AIMain::~AIMain() {
	delete aih;
}



void AIMain::InitAI(pIGlobalAICallback gcb, int team) {
	std::cout << AI_CODENAME << std::endl;

	aiInstance = aiInstances++;

	AIHelper::SetActiveInstance(aih);
	aih->Init(gcb, team);

	LOG_BASIC(aih->logger, AI_CODENAME)
}

void AIMain::ReleaseAI() {
	aiInstances--;

	AIHelper::SetActiveInstance(aih);
	aih->Release();

	if (aiInstances == 0)
	{
		Factory<LuaModule>::Free();
		Factory<AIGroup>::Free();
		Factory<AIUnit>::Free();

		Debugger::FreeInstance(Debugger::GetInstance());
	}
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
	AIHelper::SetActiveInstance(aih);

	pAIUnit unit = Factory<AIUnit>::Instance();
	unit->Reset(unitID, builderUnitID);
	aih->unitManager->AddUnit(unit);
	//aih->groupManager->AddUnit(unit);
}

void AIMain::UnitFinished(int unitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
	AIHelper::SetActiveInstance(aih);
	aih->unitManager->GetUnit(unitID)->UnitDestroyed();
}

void AIMain::UnitIdle(int unitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitMoveFailed(int unitID) {
	AIHelper::SetActiveInstance(aih);
}


void AIMain::EnemyEnterLOS(int enemyUnitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyLeaveLOS(int enemyUnitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyEnterRadar(int enemyUnitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyLeaveRadar(int enemyUnitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyDestroyed(int enemyUnitID, int attackerUnitID) {
	AIHelper::SetActiveInstance(aih);
}


void AIMain::GotChatMsg(const char* msg, int playerNum) {
	AIHelper::SetActiveInstance(aih);
}

int AIMain::HandleEvent(int msgID, const void* msgData) {
	AIHelper::SetActiveInstance(aih);

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
	AIHelper::SetActiveInstance(aih);

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
