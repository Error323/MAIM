#include "AModule.hpp"

#include "../groups/Group.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"

void AModule::SetGroup(Group *group) {
	this->group = group;
}

void AModule::UnitDestroyed(int unit) {
	units.remove(unit);
}
