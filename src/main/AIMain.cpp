#include <iostream>

#include "./AIMain.hpp"
#include "./AIHelper.hpp"
#include "./MAI.hpp"

#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../lua/AILuaCallBackHandler.hpp"
#include "../lua/AILuaModule.hpp"
#include "../groups/AIGroup.hpp"
#include "../groups/AIGroupHandler.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitHandler.hpp"
#include "../utils/ObjectFactory.hpp"
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

	LOG_BASIC(AI_CODENAME);
}

void AIMain::ReleaseAI() {
	aiInstances--;

	AIHelper::SetActiveInstance(aih);
	aih->Release();

	if (aiInstances == 0)
	{
		ObjectFactory<LuaModule>::Free();
		ObjectFactory<AIGroup>::Free();
		ObjectFactory<AIUnit>::Free();

		Debugger::FreeInstance(Debugger::GetInstance());
	}
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
	AIHelper::SetActiveInstance(aih);

	pAIUnit unit = ObjectFactory<AIUnit>::Instance();
	unit->Reset(unitID, builderUnitID);

	// TODO: use observer
	aih->aiUnitHandler->UnitCreated(unit);
	aih->aiGroupHandler->UnitCreated(unit);
}

void AIMain::UnitFinished(int unitID) {
	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
	AIHelper::SetActiveInstance(aih);

	// this reaches the unit-handler indirectly
	pAIUnit unit = aih->aiUnitHandler->GetUnit(unitID);
	unit->NotifyUnitDestroyedObservers();
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

	switch (currentFrame % 2)
	{
		case 0: // update economic state
		{
			aih->ecoState->Update();
		} break;
		case 1: // update aiGroups, calls lua modules
		{
			aih->aiGroupHandler->Update();
		} break;
	}
}
