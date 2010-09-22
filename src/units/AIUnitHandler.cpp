#include "./AIUnitHandler.hpp"
#include "./AIUnit.hpp"

#include "../utils/Debugger.hpp"

void AIUnitHandler::UnitCreated(pAIUnit unit) {
	mUnits[unit->GetID()] = unit;
	unit->AttachObserver(this);
}

void AIUnitHandler::UnitDestroyed(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	mUnits[unitID]->DetachObserver(this);
	mUnits.erase(unitID);
}

pAIUnit AIUnitHandler::GetUnit(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	return mUnits[unitID];
}
