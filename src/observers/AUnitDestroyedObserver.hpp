#ifndef UNIT_DESTROYED_OBSERVER
#define UNIT_DESTROYED_OBSERVER

#include "AObserver.hpp"

class ASubject;

class AUnitDestroyedObserver: public AObserver {
public:
	virtual ~AUnitDestroyedObserver() {}

	virtual void UnitDestroyed(int unitID) = 0;
	// override AObserver::Notify
	void Notify(ASubject* subject);

protected:
	AUnitDestroyedObserver() {}
};

#endif
