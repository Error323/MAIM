#include "./AIGroupHandler.hpp"
#include "./AIGroup.hpp"

#include "../units/AIUnit.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/ObjectFactory.hpp"

void AIGroupHandler::AddUnit(pAIUnit unit) {
	// Either find an existing group for this unit-type class...
	std::map<int, pAIGroup>::iterator i;
	for (i = mGroups.begin(); i != mGroups.end(); i++)
	{
		pAIGroup group = i->second;
		// Check for existing homogeneous group
		// And check for maximum units allowed for this group
		if (group->CanBeAdded(unit))
		{
			group->AddUnit(unit, false);
			return;
		}
	}

	// Or construct a new group
	pAIGroup group = ObjectFactory<AIGroup>::Instance();
	mGroups[group->GetID()] = group;
	group->Attach(this);
	group->AddUnit(unit, true);
}

void AIGroupHandler::Update() {
	std::map<int, pAIGroup>::iterator i;
	for (i = mGroups.begin(); i != mGroups.end(); i++)
	{
		i->second->Update();
	}
}

void AIGroupHandler::GroupDestroyed(int gid) {
	MAI_ASSERT(mGroups.find(gid) != mGroups.end());
	mGroups[gid]->Detach(this);
	mGroups.erase(gid);
}
