#include "./AIGroup.hpp"

#include "../lua/AILuaModule.hpp"
#include "../lua/AILuaModuleLoader.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Debugger.hpp"

int AIGroup::sCounter = 0;

AIGroup::AIGroup(): gid(sCounter) {
	SetGroupDestroyedSubjectID(gid); 
	sCounter++; 
	activeModule = NULL;
	modules.resize(LuaModule::LUAMODULE_NUM_PRIORITIES);
}

void AIGroup::Release() {
	activeModule = NULL;

	for (int i = 0; i < LuaModule::LUAMODULE_NUM_PRIORITIES; i++)
	{
		if (modules[i] != NULL)
		{
			modules[i]->Release();
			modules[i] = NULL;
		}
	}

	units.clear();

	NotifyGroupDestroyedObservers();
}

void AIGroup::AddUnit(pAIUnit unit, cBool isNewGroup) {
	units[unit->GetID()] = unit;

	if (isNewGroup)
	{
		// Instantiate modules for the unit
		pAIHelper aih = AIHelper::GetActiveInstance();
		pcAIUnitDef def = unit->GetUnitDef();

		AddModule(aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_EMERGENCY));
		AddModule(aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_REACTIVE));
		AddModule(aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_PROACTIVE));

		// modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY] = aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_EMERGENCY);
		// modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ] = aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_REACTIVE );
		// modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE] = aih->luaModuleLoader->GetModule(def, LuaModule::LUAMODULE_PRIORITY_PROACTIVE);
	}

	// Attach to unit subject
	unit->AttachObserver(this);
}

cBool AIGroup::CanBeAdded(pAIUnit unit) const {
	// only add unit-type classes that this group already contains
	bool canBeAdded = true;

	for (int i = 0; i < LuaModule::LUAMODULE_NUM_PRIORITIES; i++)
	{
		if (modules[i] == NULL)
			continue;

		// See if the given unit matches all modules in this group
		cUint32 typeMask    = unit->GetUnitDef()->typeMask;
		cUint32 terrainMask = unit->GetUnitDef()->terrainMask;
		cUint32 weaponMask  = unit->GetUnitDef()->weaponMask;
		cUint32 roleMask    = unit->GetUnitDef()->roleMask;

		canBeAdded = canBeAdded && modules[i]->IsSuited(typeMask, terrainMask, weaponMask, roleMask);

		// Also extract the max nr of units for this group
		canBeAdded = (canBeAdded && (units.size() < modules[i]->GetMaxGroupSize()));

		// We are very strict about this, if one module fails on either of
		// these constraints, the unit can't be added
		if (!canBeAdded)
			return false;
	}

	return true;
}

void AIGroup::AddModule(pLuaModule module) {
	cUint32 priority = module->GetPriority();

	MAI_ASSERT_MSG(priority < LuaModule::LUAMODULE_NUM_PRIORITIES);
	MAI_ASSERT_MSG(modules[priority] == NULL, "Overwriting %s with %s", modules[priority]->GetName().c_str(), module->GetName().c_str());

	module->SetGroup(this); // Allows access to this group from within the module
	modules[priority] = module; // Allows the group to select the module
}

void AIGroup::Update() {
	if (activeModule == NULL)
	{
		for (int i = 0; i < LuaModule::LUAMODULE_NUM_PRIORITIES; i++)
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
