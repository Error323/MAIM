#ifndef UNIT_DESTROYED_OBSERVER
#define UNIT_DESTROYED_OBSERVER

#include "ASubject.hpp"

class UnitDestroyedObserver: public ASubject {
	public:
		UnitDestroyedObserver() {}
		~UnitDestroyedObserver();

		void UnitDestroyed(int unit);

		int GetUnitId();

	private:
		int unit;
};

#endif
