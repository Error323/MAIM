#include "AModule.hpp"

#include "../observers/AUnitDestroyedObserver.hpp"

AModule::~AModule() {
}

void AModule::SetStack(std::stack<AModule*> &moduleStack) {
	this->moduleStack = &moduleStack;
}

void AModule::UnitDestroyed(int unit) {
	units.remove(unit);
}
