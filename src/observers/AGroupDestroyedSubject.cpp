#include "./AGroupDestroyedSubject.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../groups/AIGroup.hpp"

void AGroupDestroyedSubject::NotifyGroupDestroyedObservers() {
	// Notify all attached GroupDestroyed observers
	NotifyObservers();

	// Make groups available again
	ObjectFactory<AIGroup>::Release(dynamic_cast<pAIGroup>(this));
}
