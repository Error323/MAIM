#include "./AIMain.hpp"
#include "./AIHelper.hpp"

#include "../factories/ReusableObjectFactory.hpp"
#include "../globals/EcoState.hpp"

unsigned int AIMain::aiInstances = 0;

AIMain::AIMain(): aiInstance(0) {
}
AIMain::~AIMain() {
}



void AIMain::InitAI(IGlobalAICallback* gcb, int team) {
	aiInstance = aiInstances++;
	aih = new AIHelper();
	aih->Init(gcb, team);

	aih->ecostate->Init(aih);
}

void AIMain::ReleaseAI() {
	aiInstances--;
	aih->Release();
}



void AIMain::UnitCreated(int unitID, int builderUnitID) {
}

void AIMain::UnitFinished(int unitID) {
}

void AIMain::UnitDestroyed(int unitID, int attackerUnitID) {
}

void AIMain::UnitIdle(int unitID) {
}

void AIMain::UnitDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
}

void AIMain::EnemyDamaged(int damagedUnitID, int attackerUnitID, float damage, float3 dir) {
}

void AIMain::UnitMoveFailed(int unitID) {
}


void AIMain::EnemyEnterLOS(int enemyUnitID) {
}

void AIMain::EnemyLeaveLOS(int enemyUnitID) {
}

void AIMain::EnemyEnterRadar(int enemyUnitID) {
}

void AIMain::EnemyLeaveRadar(int enemyUnitID) {
}

void AIMain::EnemyDestroyed(int enemyUnitID, int attackerUnitID) {
}


void AIMain::GotChatMsg(const char* msg, int playerNum) {
}

int AIMain::HandleEvent(int msgID, const void* msgData) {
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
	aih->ecostate->Update();
}
