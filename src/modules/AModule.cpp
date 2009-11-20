#include "AModule.hpp"

#include "../observers/ASubject.hpp"
#include "../observers/UnitDestroyed.hpp"

~AModule::AModule() {
	unitDestroyed->Detach(this);
}

void AModule::SetStack(std::stack<AModule*> &moduleStack) {
	this->moduleStack = &moduleStack;
}

void AModule::Update(ASubject *subject) {
	if (subject == unitDestroyed) {
		int unit = unitDestroyed->GetUnitId();
		units.remove(unit);
		return;
	}
}
