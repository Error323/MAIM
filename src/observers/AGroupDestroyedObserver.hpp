#ifndef GROUP_DESTROYED_OBSERVER
#define GROUP_DESTROYED_OBSERVER

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
