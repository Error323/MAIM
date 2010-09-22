#include "AGroupDestroyedObserver.hpp"

#include "ASubject.hpp"
#include "AGroupDestroyedSubject.hpp"

void AGroupDestroyedObserver::Notify(ASubject* subject) {
	AGroupDestroyedSubject* gds = dynamic_cast<AGroupDestroyedSubject*>(subject);
	// call the observer's implementation (eg. AIGroupHandler)
	GroupDestroyed(gds->GetGroupDestroyedSubjectID());
}
