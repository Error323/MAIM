#include "AUnitDestroyedSubject.hpp"

void AUnitDestroyedSubject::UnitDestroyed() {
	Notify();
}
