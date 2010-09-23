#include "./AITaskHandler.hpp"

#include <limits>

#include "../groups/AIGroup.hpp"
#include "../groups/AIGroupHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/ObjectFactory.hpp"

Uint32 AITaskHandler::BuildTask::sBuildTaskCounter = 0;
Uint32 AITaskHandler::AttackTask::sAttackTaskCounter = 0;

void AITaskHandler::GetAttackingTargets(rvInt targetIDs) {
	std::map<cUint32, pAttackTask>::iterator i;
	for (i = mAttackTasks.begin(); i != mAttackTasks.end(); i++)
	{
		targetIDs.push_back(i->second->mTarget);
	}
}

AITaskHandler::BuildTask* AITaskHandler::GetClosestBuildTask(BuildTask::BuildTaskType btt, rcFloat3 position) {
	float smallest_distance = std::numeric_limits<float>::max();
	pBuildTask closest_build_task = NULL;

	std::map<cUint32, pBuildTask>::iterator i;
	for (i = mBuildTasks.begin(); i != mBuildTasks.end(); i++)
	{
		pBuildTask build_task = i->second;
		if (build_task->mBuildTaskType != btt)
		{
			continue;
		}

		cFloat distance = (build_task->mPosition - position).Length2D();
		if (distance < smallest_distance)
		{
			closest_build_task = build_task;
			smallest_distance = distance;
		}
	}

	return closest_build_task;
}

void AITaskHandler::AddBuildTask(pAIGroup group, BuildTask::BuildTaskType btt, pcUnitDef toBuild, rFloat3 pos) {
	MAI_ASSERT(mGroupToBuildTasks.find(group->GetID()) == mGroupToBuildTasks.end());

	group->AttachObserver(this);

	pBuildTask build_task = ObjectFactory<AITaskHandler::BuildTask>::Instance();
	build_task->mBuildTaskType = btt;
	build_task->mAlphaGroup = group;
	build_task->mPosition = pos;
	build_task->mAssisters.clear();
	mBuildTasks[build_task->mBuildTaskID] = build_task;
	mGroupToBuildTasks[group->GetID()] = build_task;
}

void AITaskHandler::AddAttackTask(pAIGroup group, int target) {
	MAI_ASSERT(mGroupToAttackTasks.find(group->GetID()) == mGroupToAttackTasks.end());

	group->AttachObserver(this);

	pAttackTask attack_task = ObjectFactory<AITaskHandler::AttackTask>::Instance();
	attack_task->mTarget = target;
	attack_task->mAlphaGroup = group;
	attack_task->mAssisters.clear();
	mAttackTasks[attack_task->mAttackTaskID] = attack_task;
	mGroupToAttackTasks[group->GetID()] = attack_task;
}

void AITaskHandler::AddAssistAttackTask(pAIGroup group, pAttackTask task) {
	MAI_ASSERT(mGroupToAttackTasks.find(group->GetID()) == mGroupToAttackTasks.end());

	group->AttachObserver(task);
	group->AttachObserver(this);

	// Assisters are also added just like normal tasks
	// This means there can exist more groups on one task
	task->mAssisters[group->GetID()] = group;
}

void AITaskHandler::AddAssistBuildTask(pAIGroup group, pBuildTask task) {
	MAI_ASSERT(mGroupToBuildTasks.find(group->GetID()) == mGroupToBuildTasks.end());

	group->AttachObserver(task);
	group->AttachObserver(this);

	// Assisters are also added just like normal tasks
	// This means there can exist more groups on one task
	task->mAssisters[group->GetID()] = group;
}

// FIXME: What todo with assisters when alphagroup is destroyed?
//        Possibly fire an observer that can be used in the lua modules
//        which allows assisting groups to determine what todo
void AITaskHandler::GroupDestroyed(int groupID) {
	std::map<Uint32, pBuildTask>::iterator build_it   = mGroupToBuildTasks.find(groupID);
	std::map<Uint32, pAttackTask>::iterator attack_it = mGroupToAttackTasks.find(groupID);

	pAIGroup group = AIHelper::GetActiveInstance()->GetAIGroupHandler()->GetGroup(groupID);

	if (build_it != mGroupToBuildTasks.end())
	{
		pBuildTask build_task = build_it->second;

		mGroupToBuildTasks.erase(groupID);
		group->DetachObserver(this);

		if (group == build_task->mAlphaGroup)
		{
			mBuildTasks.erase(build_task->mBuildTaskID);
			ObjectFactory<AITaskHandler::BuildTask>::Release(build_task);
		}
		else { /* we were an assisting group */ }
	}
	else if (attack_it != mGroupToAttackTasks.end())
	{
		pAttackTask attack_task = attack_it->second;

		mGroupToAttackTasks.erase(groupID);
		group->DetachObserver(this);

		if (group == attack_task->mAlphaGroup)
		{
			mAttackTasks.erase(attack_task->mAttackTaskID);
			ObjectFactory<AITaskHandler::AttackTask>::Release(attack_task);
		}
		else { /* we were an assisting group */ }
	}
	else
	{
		MAI_ASSERT_MSG(false, "Group(%d) is not achieving a task", groupID);
	}
}

void AITaskHandler::AttackTask::GroupDestroyed(int groupID) {
	MAI_ASSERT(mAssisters.find(groupID) != mAssisters.end());
	pAIGroup group = AIHelper::GetActiveInstance()->GetAIGroupHandler()->GetGroup(groupID);
	group->DetachObserver(this);
	mAssisters.erase(groupID);
}

void AITaskHandler::BuildTask::GroupDestroyed(int groupID) {
	MAI_ASSERT(mAssisters.find(groupID) != mAssisters.end());
	pAIGroup group = AIHelper::GetActiveInstance()->GetAIGroupHandler()->GetGroup(groupID);
	group->DetachObserver(this);
	mAssisters.erase(groupID);
}
