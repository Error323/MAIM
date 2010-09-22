#ifndef GROUP_DESTROYED_SUBJECT_HDR
#define GROUP_DESTROYED_SUBJECT_HDR

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
