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

unsigned int AIGroup::sGroupCounter = 0;

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

void AIGroup::AddUnit(pAIUnit unit, bool isNewGroup) {
	units[unit->GetID()] = unit;

	if (isNewGroup)
	{
		// Instantiate modules for the unit
		pAIHelper aih = AIHelper::GetActiveInstance();
		pcAIUnitDef def = unit->GetUnitDef();

		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_EMERGENCY));
		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_REACTIVE));
		AddModule(aih->GetLuaModuleLoader()->GetModule(def, LuaModule::LUAMODULE_PRIORITY_PROACTIVE));
	}

	// Attach to unit subject
	unit->AttachObserver(this);

	if (modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY]->AddUnit(unit->GetID()); }
	if (modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ]->AddUnit(unit->GetID()); }
	if (modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE]->AddUnit(unit->GetID()); }
}

bool AIGroup::CanAddUnit(pAIUnit unit) const {
	// only add unit-type classes that this group already contains
	bool canAddUnit = true;

	const AIUnitDef::AIUnitDefClass& unitDefClass = unit->GetUnitDef()->unitDefClass;

	for (int i = 0; i < LuaModule::LUAMODULE_NUM_PRIORITIES; i++)
	{
		if (modules[i] == NULL)
			continue;

		const AIUnitDef::AIUnitDefClass& groupDefClass = modules[i]->GetUnitDefClass();

		// See if the given unit matches all modules in this group
		canAddUnit = util::IsBinaryMatch(unitDefClass, groupDefClass);

		// Also extract the maximum number of units for this group
		canAddUnit = canAddUnit && (units.size() < modules[i]->GetMaxGroupSize());

		// We are very strict about this: if one module fails on
		// any of these constraints, the unit can't be added
		if (!canAddUnit)
			return false;
	}

	return true;
}

void AIGroup::UnitDestroyed(unsigned int unitID) {
	MAI_ASSERT(units.find(unitID) != units.end());

	pAIUnit unit = units[unitID];

	if (modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_EMERGENCY]->DelUnit(unit->GetID()); }
	if (modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_REACTIVE ]->DelUnit(unit->GetID()); }
	if (modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE] != NULL) { modules[LuaModule::LUAMODULE_PRIORITY_PROACTIVE]->DelUnit(unit->GetID()); }

	unit->DetachObserver(this);
	units.erase(unitID);

	if (units.empty())
		Release();
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



std::ostream& operator << (std::ostream& out, rcAIGroup group) {
	out << "\nGroup{id: ";
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
