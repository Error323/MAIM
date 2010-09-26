#ifndef UNIT_DESTROYED_OBSERVER_HDR
#define UNIT_DESTROYED_OBSERVER_HDR

#include "AObserver.hpp"

class ASubject;

class AUnitDestroyedObserver: public AObserver {
public:
	virtual ~AUnitDestroyedObserver() {}

	virtual void UnitDestroyed(unsigned int unitID) = 0;
	// override AObserver::Notify
	void Notify(ASubject* subject);

protected:
	AUnitDestroyedObserver() {}
};

#endif
