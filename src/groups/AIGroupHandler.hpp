#ifndef AI_GROUP_HANDLER_HDR
#define AI_GROUP_HANDLER_HDR

#include "../main/Types.hpp"
#include "../observers/AGroupDestroyedObserver.hpp"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIUnit)

class AIGroupHandler: public AGroupDestroyedObserver {
public:
	AIGroupHandler() {}
	~AIGroupHandler() {}

	void     UnitFinished(pAIUnit);
	pAIGroup GetGroup(int groupID);
	void     Update();

private:
	std::map<int, pAIGroup> mGroups;

	void GroupDestroyed(int gid);
};

#endif

