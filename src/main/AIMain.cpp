#include <iostream>

#include "./MAI.hpp"
#include "./AIMain.hpp"
#include "./AIHelper.hpp"

#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../globals/AITaskHandler.hpp"
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
	aiInstance = aiInstances++;

	AIHelper::SetActiveInstance(aih);
	aih->Init(gcb, team);

	std::stringstream initMsg;
		initMsg << "[InitAI] " << AI_CODENAME << " " << AI_CREDITS;
		initMsg << (aih->Initialized()? " initialized succesfully": " failed to initialize");
		initMsg << "\n\n";

	aih->GetLogger()->Log(initMsg.str());
	aih->GetCallbackHandler()->SendTextMsg((initMsg.str()).c_str(), 0);
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
		ObjectFactory<AITaskHandler::AttackTask>::Free();
		ObjectFactory<AITaskHandler::BuildTask>::Free();

		Debugger::FreeInstance(Debugger::GetInstance());
	}
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);

	pAIUnit unit = ObjectFactory<AIUnit>::Instance();

	unit->Init(unitID, builderUnitID);
	aih->GetAIUnitHandler()->UnitCreated(unit);
}

void AIMain::UnitFinished(int unitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);

	pAIUnit unit = aih->GetAIUnitHandler()->GetUnit(unitID);

	aih->GetAIGroupHandler()->UnitFinished(unit);
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);

	// this reaches the unit-handler indirectly
	pAIUnit unit = aih->GetAIUnitHandler()->GetUnit(unitID);
	unit->NotifyUnitDestroyedObservers();
}

void AIMain::UnitIdle(int unitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::UnitMoveFailed(int unitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}


void AIMain::EnemyEnterLOS(int enemyUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyLeaveLOS(int enemyUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyEnterRadar(int enemyUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyLeaveRadar(int enemyUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

void AIMain::EnemyDestroyed(int enemyUnitID, int attackerUnitID) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}


void AIMain::GotChatMsg(const char* msg, int playerNum) {
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);
}

int AIMain::HandleEvent(int msgID, const void* msgData) {
	if (!aih->Initialized()) {
		return 0;
	}

	AIHelper::SetActiveInstance(aih);

	switch (msgID)
	{
		case AI_EVENT_UNITGIVEN:
		{
			const ChangeTeamEvent* cte = (const ChangeTeamEvent*) msgData;
			UnitCreated(cte->unit, -1);
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
	if (!aih->Initialized()) {
		return;
	}

	AIHelper::SetActiveInstance(aih);

	// first Update event is at frame 1, not 0
	aih->SetCurrFrame(aih->GetCurrFrame() + 1);

	switch (aih->GetCurrFrame() % 2)
	{
		case 0: // update economic state
		{
			aih->GetEcoState()->Update();
		} break;
		case 1: // update aiGroups, calls lua modules
		{
			aih->GetAIGroupHandler()->Update();
		} break;
	}
}
