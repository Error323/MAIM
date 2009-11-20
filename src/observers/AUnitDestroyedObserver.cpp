#include "AUnitDestroyedObserver.hpp"

#include "ASubject.hpp"
#include "AUnitDestroyedSubject.hpp"

void AUnitDestroyedObserver::Update(ASubject *subject) {
	AUnitDestroyedSubject *uds = dynamic_cast<AUnitDestroyedSubject*>(subject);
	UnitDestroyed(uds->unit);
}
