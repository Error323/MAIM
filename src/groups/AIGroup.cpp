#include "./AIGroup.hpp"

#include "../lua/AILuaModule.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Debugger.hpp"

int AIGroup::sCounter = 0;

void AIGroup::Release() {
	std::list<pLuaModule>::iterator i;

	for (i = modules.begin(); i != modules.end(); i++)
		(*i)->Release();

	modules.clear();
	units.clear();

	while (!moduleStack.empty())
		moduleStack.pop();

	NotifyGroupDestroyedObservers();
}

void AIGroup::AddUnit(pAIUnit unit, cBool isNewGroup) {
	units[unit->GetID()] = unit;
	if (isNewGroup)
	{
		// Instantiate modules for the unit
		
	}
	// Attach to unit subject
	unit->Attach(this);
}

cBool AIGroup::CanBeAdded(pAIUnit unit) const {
	// only add unit-type classes that this group already contains
	bool canBeAdded = true;

	std::list<pLuaModule>::const_iterator i;
	for (i = modules.begin(); i != modules.end(); i++)
	{
		// See if the given unit matches all modules in this group
		Uint32 typeMask     = unit->GetUnitDef()->typeMask;
		Uint32 terrainMask  = unit->GetUnitDef()->terrainMask;
		Uint32 weaponMask   = unit->GetUnitDef()->weaponMask;
		Uint32 moveDataMask = unit->GetUnitDef()->boMoveDataMask;
		canBeAdded = canBeAdded && (*i)->IsSuited(typeMask, terrainMask, weaponMask, moveDataMask);

		// Also extract the max nr of units for this group
		canBeAdded = canBeAdded && units.size() < (*i)->GetMaxGroupSize();

		// We are very strict about this, if one module fails on either of
		// these constraints, the unit can't be added
		if (!canBeAdded)
			return false;
	}

	return true;
}

// Make sure to add modules in this order: emergencies, reactives, proactives
void AIGroup::AddModule(pLuaModule module) {
	module->SetGroup(this); // Allows access to this group from within the module
	modules.push_back(module); // Allows the group to select the module
}

void AIGroup::PushModule(pLuaModule module) {
	moduleStack.push(module);
}

void AIGroup::Update() {
	if (moduleStack.empty()) {
		std::list<pLuaModule>::iterator i;
		for (i = modules.begin(); i != modules.end(); i++) {
			if ((*i)->CanRun()) {
				PushModule(*i);
				break;
			}
		}
	}

	// Module::Update() returns true when done
	while (!moduleStack.empty() && moduleStack.top()->Update())
		moduleStack.pop();
}

void AIGroup::UnitDestroyed(int unit) {
	MAI_ASSERT(units.find(unit) != units.end());
	units[unit]->Detach(this);
	units.erase(unit);
	if (units.empty())
		Release();
}
