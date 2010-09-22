#include "./AUnitDestroyedSubject.hpp"
#include "../utils/Factory.hpp"
#include "../units/AIUnit.hpp"

void AUnitDestroyedSubject::UnitDestroyed() {
	// Notify all attached UnitDestroyed observers
	Notify();

	// Make unit available again
	Factory<AIUnit>::Release(dynamic_cast<pAIUnit>(this));
}
