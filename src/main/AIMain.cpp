#include "./AIMain.hpp"
#include "./AIHelper.hpp"
#include "../lua/LuaAICallBackHandler.hpp"

#include "../factories/ReusableObjectFactory.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

unsigned int AIMain::aiInstances = 0;

AIMain::AIMain(): aiInstance(0) {
}
AIMain::~AIMain() {
}



void AIMain::InitAI(IGlobalAICallback* gcb, int team) {
	aiInstance = aiInstances++;
	aih = new AIHelper();
	aih->Init(gcb, team);
}

void AIMain::ReleaseAI() {
	aiInstances--;
	aih->Release();
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::UnitFinished(int unitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::UnitIdle(int unitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::UnitDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::EnemyDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::UnitMoveFailed(int unitID) {
	LuaAICallBackHandler::SetHelper(aih);
}


void AIMain::EnemyEnterLOS(int enemyUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::EnemyLeaveLOS(int enemyUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::EnemyEnterRadar(int enemyUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::EnemyLeaveRadar(int enemyUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}

void AIMain::EnemyDestroyed(int enemyUnitID, int attackerUnitID) {
	LuaAICallBackHandler::SetHelper(aih);
}


void AIMain::GotChatMsg(const char* msg, int playerNum) {
	LuaAICallBackHandler::SetHelper(aih);
}

int AIMain::HandleEvent(int msgID, const void* msgData) {
	LuaAICallBackHandler::SetHelper(aih);

	switch (msgID) {
		case AI_EVENT_UNITGIVEN: {
			const ChangeTeamEvent* cte = (const ChangeTeamEvent*) msgData;
			UnitFinished(cte->unit);
		} break;
		case AI_EVENT_UNITCAPTURED: {
			const ChangeTeamEvent* cte = (const ChangeTeamEvent*) msgData;
			UnitDestroyed(cte->unit, -1);
		} break;
	}

	return 0;
}


void AIMain::Update() {
	LuaAICallBackHandler::SetHelper(aih);
	aih->ecoState->Update();
}
