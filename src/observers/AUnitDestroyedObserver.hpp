#ifndef UNIT_DESTROYED_OBSERVER
#define UNIT_DESTROYED_OBSERVER

#include "AObserver.hpp"

class ASubject;

class AUnitDestroyedObserver: public AObserver {
	public:
		virtual ~AUnitDestroyedObserver() {}

		virtual void UnitDestroyed(int unit) = 0;
		void Update(ASubject *subject);

	protected:
		AUnitDestroyedObserver() {}
};

#endif
