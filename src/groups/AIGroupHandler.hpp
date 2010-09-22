#ifndef AI_GROUP_HANDLER_HDR
#define AI_GROUP_HANDLER_HDR

#include "../main/Types.hpp"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIUnit)

class AIGroupHandler {
public:
	AIGroupHandler() {}
	~AIGroupHandler() {}

	void AddUnit(pAIUnit);
	void Update();

private:
	std::map<int, pAIGroup> mGroups;
};

#endif

