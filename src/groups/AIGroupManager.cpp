#include "./AIGroupManager.hpp"
#include "./AIGroup.hpp"

#include "../units/AIUnit.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Factory.hpp"

void AIGroupManager::AddUnit(pAIUnit unit) {
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
	pAIGroup group = Factory<AIGroup>::Instance();
	group->AddUnit(unit, true);
}

void AIGroupManager::Update() {
	std::map<int, pAIGroup>::iterator i;
	for (i = mGroups.begin(); i != mGroups.end(); i++)
	{
		i->second->Update();
	}
}
