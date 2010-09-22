#include "AUnitDestroyedObserver.hpp"

#include "ASubject.hpp"
#include "AUnitDestroyedSubject.hpp"

void AUnitDestroyedObserver::Notify(ASubject* subject) {
	AUnitDestroyedSubject* uds = dynamic_cast<AUnitDestroyedSubject*>(subject);
	// call the observer's implementation (eg. AIUnitHandler)
	UnitDestroyed(uds->GetUnitDestroyedSubjectID());
}
