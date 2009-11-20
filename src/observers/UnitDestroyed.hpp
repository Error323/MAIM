#ifndef UNIT_DESTROYED
#define UNIT_DESTROYED

#include "ASubject.hpp"

class UnitDestroyed: public ASubject {
	public:
		UnitDestroyed();
		~UnitDestroyed();

		void UnitDestroyed(int unit);
		int GetUnitId();

	private:
		int unit;
};

#endif
