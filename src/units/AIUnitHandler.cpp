#include "./AIUnitHandler.hpp"
#include "./AIUnit.hpp"

#include "../utils/Debugger.hpp"

void AIUnitHandler::AddUnit(pAIUnit unit) {
	mUnits[unit->GetID()] = unit;
	unit->AttachObserver(this);
}

pAIUnit AIUnitHandler::GetUnit(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	return mUnits[unitID];
}

void AIUnitHandler::UnitDestroyed(int unitID) {
	MAI_ASSERT(mUnits.find(unitID) != mUnits.end());
	mUnits[unitID]->DetachObserver(this);
	mUnits.erase(unitID);
}
