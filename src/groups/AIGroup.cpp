#include "./AIGroup.hpp"

#include "../utils/Factory.hpp"
#include "../lua/AILuaModule.hpp"
#include "../units/AIUnit.hpp"

int AIGroup::sCounter = 0;

void AIGroup::Release() {
	std::list<pLuaModule>::iterator i;

	for (i = modules.begin(); i != modules.end(); i++)
		(*i)->Release();

	modules.clear();
	units.clear();

	while (!moduleStack.empty())
		moduleStack.pop();

	Factory<AIGroup>::Release(this);
}

void AIGroup::AddUnit(pAIUnit unit, cBool isNewGroup) {
	units[unit->GetID()] = unit;
	if (isNewGroup)
	{
		// Instantiate modules for the unit
	}
}

cBool AIGroup::CanBeAdded(pAIUnit) const {
	// Should check wether unit-type classes match *EXACTLY* with the
	// unit-type classes that this group already contains
	return false;
}

// Make sure to add modules in this order: emergencies, reactives, proactives
void AIGroup::AddModule(pLuaModule module) {
	module->SetGroup(this); // Allows access to this group from within the module
	module->Filter(units); // Determines which units are suited for this module
	modules.push_back(module); // Allows the group to select the module
}

void AIGroup::RemoveModule(pLuaModule module) {
	modules.remove(module);
	module->Release();
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
	//TODO: Check for module removal for this unit class
	units.erase(unit);
	if (units.empty())
		Release();
}
