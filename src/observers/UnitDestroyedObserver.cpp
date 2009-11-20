#include "UnitDestroyedObserver.hpp"

void UnitDestroyedObserver::UnitDestroyed(int unit) {
	this->unit = unit;
	Notify();
}

int UnitDestroyedObserver::GetUnitId() {
	return unit;
}
