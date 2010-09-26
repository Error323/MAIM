#include "./AIGroupHandler.hpp"
#include "./AIGroup.hpp"

#include "../main/AIHelper.hpp"
#include "../units/AIUnit.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ObjectFactory.hpp"

void AIGroupHandler::UnitFinished(pAIUnit unit) {
	// Either find an existing group for this unit-type class...
	std::map<int, pAIGroup>::iterator i;
	for (i = mGroups.begin(); i != mGroups.end(); i++)
	{
		pAIGroup group = i->second;
		// Check for existing homogeneous group
		// And check for maximum units allowed for this group
		if (group->CanAddUnit(unit))
		{
			group->AddUnit(unit, false);
			return;
		}
	}

	// Or construct a new group
	pAIGroup group = ObjectFactory<AIGroup>::Instance();
	mGroups[group->GetID()] = group;
	group->AttachObserver(this);
	group->AddUnit(unit, true);
}

void AIGroupHandler::Update() {
	std::map<int, pAIGroup>::iterator i;
	for (i = mGroups.begin(); i != mGroups.end(); i++)
	{
		i->second->Update();
	}
}

pAIGroup AIGroupHandler::GetGroup(int groupID) {
	MAI_ASSERT(mGroups.find(groupID) != mGroups.end());
	return mGroups[groupID];
}

void AIGroupHandler::GroupDestroyed(int gid) {
	MAI_ASSERT(mGroups.find(gid) != mGroups.end());
	mGroups[gid]->DetachObserver(this);
	mGroups.erase(gid);
}
