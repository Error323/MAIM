#include "./AIGroup.hpp"

#include "../lua/AILuaModule.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Debugger.hpp"

int AIGroup::sCounter = 0;

AIGroup::AIGroup(): gid(sCounter) {
	SetGroupDestroyedSubjectID(gid); 
	sCounter++; 
	activeModule = NULL;
	modules.resize(MODULE_PRIORITY_COUNT);
}

void AIGroup::Release() {
	for (int i = 0; i < MODULE_PRIORITY_COUNT; i++)
	{
		if (modules[i] != NULL)
		{
			modules[i]->Release();
			modules[i] = NULL;
		}
	}

	units.clear();
	activeModule = NULL;

	NotifyGroupDestroyedObservers();
}

void AIGroup::AddUnit(pAIUnit unit, cBool isNewGroup) {
	units[unit->GetID()] = unit;
	if (isNewGroup)
	{
		// Instantiate modules for the unit
		
	}
	// Attach to unit subject
	unit->AttachObserver(this);
}

cBool AIGroup::CanBeAdded(pAIUnit unit) const {
	// only add unit-type classes that this group already contains
	bool canBeAdded = true;

	for (int i = 0; i < MODULE_PRIORITY_COUNT; i++)
	{
		// See if the given unit matches all modules in this group
		Uint32 typeMask     = unit->GetUnitDef()->typeMask;
		Uint32 terrainMask  = unit->GetUnitDef()->terrainMask;
		Uint32 weaponMask   = unit->GetUnitDef()->weaponMask;
		Uint32 moveDataMask = unit->GetUnitDef()->boMoveDataMask;
		canBeAdded = canBeAdded && modules[i]->IsSuited(typeMask, terrainMask, weaponMask, moveDataMask);

		// Also extract the max nr of units for this group
		canBeAdded = canBeAdded && units.size() < modules[i]->GetMaxGroupSize();

		// We are very strict about this, if one module fails on either of
		// these constraints, the unit can't be added
		if (!canBeAdded)
			return false;
	}

	return true;
}

void AIGroup::AddModule(pLuaModule module) {
	module->SetGroup(this); // Allows access to this group from within the module
	modules[module->GetPriority()] = module; // Allows the group to select the module
}

void AIGroup::Update() {
	if (activeModule == NULL)
	{
		for (int i = 0; i < MODULE_PRIORITY_COUNT; i++)
		{
			if (modules[i] != NULL && modules[i]->CanRun()) 
			{
				activeModule = modules[i];
				break;
			}
		}
	}

	// Module::Update() returns true when done
	if (activeModule != NULL && activeModule->Update())
		activeModule = NULL;
}

void AIGroup::UnitDestroyed(int unit) {
	MAI_ASSERT(units.find(unit) != units.end());
	units[unit]->DetachObserver(this);
	units.erase(unit);
	if (units.empty())
		Release();
}
