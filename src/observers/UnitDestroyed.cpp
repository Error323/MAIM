#include "UnitDestroyed.hpp"

void UnitDestroyed::UnitDestroyed(int unit) {
	this->unit = unit;
	Notify();
}

int UnitDestroyed::GetUnitId() {
	return unit;
}
