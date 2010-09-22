#ifndef GROUP_DESTROYED_SUBJECT
#define GROUP_DESTROYED_SUBJECT

#include "ASubject.hpp"

class AGroupDestroyedSubject: public ASubject {
public:
	AGroupDestroyedSubject() {}
	~AGroupDestroyedSubject() {}

	void NotifyGroupDestroyedObservers();
	void SetGroupDestroyedSubjectID(int id) { groupID = id;   }
	int  GetGroupDestroyedSubjectID()       { return groupID; }

private:
	int groupID;
};

#endif
