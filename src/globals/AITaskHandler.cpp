#include "./AITaskHandler.hpp"

#include "../groups/AIGroup.hpp"
#include "../groups/AIGroupHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/ObjectFactory.hpp"

void AITaskHandler::AddBuildTask(pAIGroup group, BuildTask::BuildTaskType btt, pcUnitDef toBuild, rFloat3 pos) {
	MAI_ASSERT(mBuildTasks.find(group->GetID()) == mBuildTasks.end());

	group->AttachObserver(this);

	pBuildTask build_task = ObjectFactory<AITaskHandler::BuildTask>::Instance();
	build_task->mBuildTaskType = btt;
	build_task->mAlphaGroup = group;
	build_task->mPosition = pos;
	build_task->mAssisters.clear();
	mBuildTasks[group->GetID()] = build_task;
}

void AITaskHandler::AddAttackTask(pAIGroup group, int target) {
	MAI_ASSERT(mAttackTasks.find(group->GetID()) == mAttackTasks.end());

	group->AttachObserver(this);

	pAttackTask attack_task = ObjectFactory<AITaskHandler::AttackTask>::Instance();
	attack_task->mTarget = target;
	attack_task->mAlphaGroup = group;
	attack_task->mAssisters.clear();
	mAttackTasks[group->GetID()] = attack_task;
}

void AITaskHandler::AddAssistAttackTask(pAIGroup group, pAttackTask task) {
	MAI_ASSERT(mAttackTasks.find(group->GetID()) == mAttackTasks.end());

	group->AttachObserver(task);
	group->AttachObserver(this);

	// Assisters are also added just like normal tasks
	// This means there can exist more groups on one task
	task->mAssisters[group->GetID()] = group;
}

void AITaskHandler::AddAssistBuildTask(pAIGroup group, pBuildTask task) {
	MAI_ASSERT(mBuildTasks.find(group->GetID()) == mBuildTasks.end());

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
	std::map<Uint32, pBuildTask>::iterator build_it   = mBuildTasks.find(groupID);
	std::map<Uint32, pAttackTask>::iterator attack_it = mAttackTasks.find(groupID);

	pAIGroup group = AIHelper::GetActiveInstance()->GetAIGroupHandler()->GetGroup(groupID);

	if (build_it != mBuildTasks.end())
	{
		pBuildTask build_task = build_it->second;

		mBuildTasks.erase(groupID);
		group->DetachObserver(this);

		if (group == build_task->mAlphaGroup)
		{
			ObjectFactory<AITaskHandler::BuildTask>::Release(build_task);
		}
		else { /* we were an assisting group */ }
	}
	else if (attack_it != mAttackTasks.end())
	{
		pAttackTask attack_task = attack_it->second;

		mAttackTasks.erase(groupID);
		group->DetachObserver(this);

		if (group == attack_task->mAlphaGroup)
		{
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
