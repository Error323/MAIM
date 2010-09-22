#include "AGroupDestroyedObserver.hpp"

#include "ASubject.hpp"
#include "AGroupDestroyedSubject.hpp"

void AGroupDestroyedObserver::Notify(ASubject* subject) {
	AGroupDestroyedSubject* uds = dynamic_cast<AGroupDestroyedSubject*>(subject);
	// call the observer's implementation (eg. AIGroupHandler)
	GroupDestroyed(uds->GetGroupDestroyedSubjectID());
}
