#ifndef UNIT_DESTROYED_SUBJECT
#define UNIT_DESTROYED_SUBJECT

#include "ASubject.hpp"

class AUnitDestroyedSubject: public ASubject {
public:
	AUnitDestroyedSubject() {}
	~AUnitDestroyedSubject() {}

	void NotifyUnitDestroyedObservers();
	void SetUnitDestroyedSubjectID(int id) { unitID = id;   }
	int  GetUnitDestroyedSubjectID()       { return unitID; }

private:
	int unitID;
};

#endif
