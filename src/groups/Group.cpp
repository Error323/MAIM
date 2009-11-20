#include "Group.hpp"

#include "../observers/ASubject.hpp"
#include "../observers/UnitDestroyedObserver.hpp"
#include "../modules/AModule.hpp"

void Group::Release() {
	std::list<AModule*>::iterator i;
	for (i = modules.begin(); i != modules.end(); i++)
		(*i)->Release(); // make available in factory again
	modules.clear();
	units.clear();
	while(!moduleStack.empty())
		moduleStack.pop();
}

void Group::AddUnit(int unit) {
	units.push_back(unit);
	std::list<AModule*>::iterator i;
	for (i = modules.begin(); i != modules.end(); i++)
		(*i)->Filter(units);
}

// Make sure to add modules in this order: emergencies, reactives, proactives
void Group::AddModule(AModule *module) {
	module->Filter(units); // Determines which units are good for this module
	module->SetStack(moduleStack); // Allows the stack to be filled with this module
	modules.push_back(module); // Allows the group to select the module
}

void Group::RemoveModule(AModule *module) {
	modules.remove(module);
	module->Release();
}

void Group::Update() {
	if (moduleStack.empty()) {
		std::list<AModule*>::iterator i;
		for (i = modules.begin(); i != modules.end(); i++) {
			if ((*i)->CanRun()) {
				moduleStack.push(*i);
				break;
			}
		}
	}

	// Module::Update() returns true when done
	while(!moduleStack.empty() && moduleStack.top()->Update())
		moduleStack.pop();
}

void Group::Update(ASubject *subject) {
	if (subject == unitDestroyed) {
		int unit = unitDestroyed->GetUnitId();
		units.remove(unit);
		if (units.empty())
			Release();
		return;
	}
}
