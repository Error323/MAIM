#include "./AUnitDestroyedSubject.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../units/AIUnit.hpp"

void AUnitDestroyedSubject::NotifyUnitDestroyedObservers() {
	// Notify all attached UnitDestroyed observers
	NotifyObservers();

	// Make unit available again
	ObjectFactory<AIUnit>::Release(dynamic_cast<pAIUnit>(this));
}
