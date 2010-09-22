#ifndef GROUP_DESTROYED_OBSERVER_HDR
#define GROUP_DESTROYED_OBSERVER_HDR

#include "AObserver.hpp"

class ASubject;

class AGroupDestroyedObserver: public AObserver {
public:
	virtual ~AGroupDestroyedObserver() {}

	virtual void GroupDestroyed(int unitID) = 0;
	// override AObserver::Notify
	void Notify(ASubject* subject);

protected:
	AGroupDestroyedObserver() {}
};

#endif
