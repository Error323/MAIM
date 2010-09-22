#ifndef AI_GROUP_MANAGER_HDR
#define AI_GROUP_MANAGER_HDR

#include "../main/Types.hpp"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIUnit)

class AIGroupManager {
public:
	AIGroupManager() {}
	~AIGroupManager() {}

	void AddUnit(pAIUnit);
	void Update();

private:
	std::map<int, pAIGroup> mGroups;
};

#endif // AI_GROUP_MANAGER_HDR

