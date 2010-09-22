#include "./AIUnitManager.hpp"
#include "./AIUnit.hpp"

#include "../utils/Debugger.hpp"

void AIUnitManager::AddUnit(pAIUnit unit) {
	mUnits[unit->GetID()] = unit;
	unit->Attach(this);
}

pAIUnit AIUnitManager::GetUnit(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	return mUnits[unitID];
}

void AIUnitManager::UnitDestroyed(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	mUnits.erase(unitID);
}
