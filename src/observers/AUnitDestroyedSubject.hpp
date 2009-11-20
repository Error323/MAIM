#ifndef UNIT_DESTROYED_SUBJECT
#define UNIT_DESTROYED_SUBJECT

#include "ASubject.hpp"

class AUnitDestroyedSubject: public ASubject {
	public:
		AUnitDestroyedSubject(int _unit): unit(_unit){}
		~AUnitDestroyedSubject(){}

		void UnitDestroyed();

		int unit;
};

#endif
