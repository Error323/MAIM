#ifndef UNIT_DESTROYED_SUBJECT
#define UNIT_DESTROYED_SUBJECT

#include "ASubject.hpp"

class AUnitDestroyedSubject: public ASubject {
public:
	AUnitDestroyedSubject(){}
	~AUnitDestroyedSubject(){}

	void UnitDestroyed();
	void SetUnitDestroyedId(int id) { unitID = id;}
	int  GetUnitDestroyedId()       { return unitID; }

private:
	int unitID;
};

#endif
