#include "./AIGroup.hpp"

#include <sstream>

#include "../lua/AILuaModule.hpp"
#include "../lua/AILuaModuleLoader.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Util.hpp"

int AIGroup::sGroupCounter = 0;

AIGroup::AIGroup(): groupID(sGroupCounter) {
	SetGroupDestroyedSubjectID(groupID); 
	sGroupCounter++; 
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

		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_EMERGENCY));
		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_REACTIVE));
		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_PROACTIVE));

		// modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY] = aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_EMERGENCY);
		// modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ] = aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_REACTIVE );
		// modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE] = aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_PROACTIVE);
	}

	// Attach to unit subject
	unit->AttachObserver(this);
}

cBool AIGroup::CanBeAdded(pAIUnit unit) const {
	// only add unit-type classes that this group already contains
	bool canBeAdded = true;

	LuaModule::LuaModuleClass ulmc;
		ulmc.typeMask = unit->GetUnitDef()->typeMask;
		ulmc.terrMask = unit->GetUnitDef()->terrainMask;
		ulmc.weapMask = unit->GetUnitDef()->weaponMask;
		ulmc.roleMask = unit->GetUnitDef()->roleMask;

	for (int i = 0; i < LuaModule::LUAMODULE_NUM_PRIORITIES; i++)
	{
		if (modules[i] == NULL)
			continue;

		const LuaModule::LuaModuleClass& glmc = modules[i]->GetModuleClass();

		// See if the given unit matches all modules in this group
		canBeAdded = canBeAdded && IS_BINARY_SUBSET(ulmc.typeMask, glmc.typeMask);
		canBeAdded = canBeAdded && IS_BINARY_SUBSET(ulmc.terrMask, glmc.terrMask);
		canBeAdded = canBeAdded && IS_BINARY_SUBSET(ulmc.weapMask, glmc.weapMask);
		canBeAdded = canBeAdded && IS_BINARY_SUBSET(ulmc.roleMask, glmc.roleMask);

		// Also extract the maximum number of units for this group
		canBeAdded = (canBeAdded && (units.size() < modules[i]->GetMaxGroupSize()));

		// We are very strict about this: if one module fails on
		// any of these constraints, the unit can't be added
		if (!canBeAdded)
			return false;
	}

	return true;
}

void AIGroup::AddModule(pLuaModule module) {
	cUint32 priority = module->GetPriority();

	MAI_ASSERT_MSG(priority < LuaModule::LUAMODULE_NUM_PRIORITIES, "Illegal module priority %u", priority);
	MAI_ASSERT_MSG(modules[priority] == NULL, "Overwriting %s with %s", modules[priority]->GetName().c_str(), module->GetName().c_str());

	modules[priority] = module; // Allows the group to select the module
	module->SetGroup(this); // Allows access to this group from within the module
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
	
	// Update all units in the group (age, position etc)
	for (std::map<int, pAIUnit>::iterator i = units.begin(); i != units.end(); i++)
	{
		i->second->Update();
	}
}

void AIGroup::UnitDestroyed(int unit) {
	MAI_ASSERT(units.find(unit) != units.end());
	units[unit]->DetachObserver(this);
	units.erase(unit);
	if (units.empty())
		Release();
}

std::ostream& operator<<(std::ostream &out, rcAIGroup group) {
	out << "\nGroup{id:";
	out << group.groupID;
	out << "}\n\n";

	for (Uint32 i = 0; i < group.modules.size(); i++)
	{
		if (group.modules[i] == NULL)
			out << "\tNone\n";
		else
			out << "\t" << (*group.modules[i]) << "\n";
	}

	out << "\n\tActive: ";
	if (group.activeModule == NULL)
		out << "None";
	else
		out << (*group.activeModule);
	out << "\n\n";

	std::map<int, pAIUnit>::const_iterator i;
	for (i = group.units.begin(); i != group.units.end(); i++)
	{
		out << "\t" << *(i->second) << "\n";
	}

	out << "\n";

	return out;
}
