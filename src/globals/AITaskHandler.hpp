#ifndef TASK_HOLDER
#define TASK_HOLDER

#include <list>
#include <map>
#include <iostream>

#include "../main/Types.hpp"
#include "../observers/AGroupDestroyedObserver.hpp"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIHelper)
DECLARE_CLASS(AIUnitDef)

class AITaskHandler: public AGroupDestroyedObserver {
public:
	AITaskHandler(){}
	~AITaskHandler(){}

	enum TaskType { 
		BUILD, 
		ATTACK
	};

	DECLARE_STRUCT(AttackTask)
	DECLARE_STRUCT(BuildTask)

	struct BuildTask: public AGroupDestroyedObserver {
		enum BuildTaskType { 
			FACTORY, 
			METAL, 
			ENERGY, 
			DEFENSE 
		};

		BuildTask(){}

		/** The abstract build */
		BuildTaskType mBuildTaskType;

		/** The unit to build */
		pUnitDef mUnitToBuild;

		/** The primary group that applies the build */
		pAIGroup mAlphaGroup;

		/** The assisters of the build */
		std::map<Uint32, pAIGroup> mAssisters;

		/** The position where the build will take place */
		float3 mPosition;

		// Implementation for assisters
		void GroupDestroyed(int groupID);

		/** The toString() function */
		friend std::ostream& operator<<(std::ostream&, rcBuildTask);
	};

	struct AttackTask: public AGroupDestroyedObserver {
		AttackTask(){}

		/** The target to attack */
		int mTarget;

		/** The primary attack group */
		pAIGroup mAlphaGroup;

		/** The assisters of the attack */
		std::map<Uint32, pAIGroup> mAssisters;

		// Implementation for assisters
		void GroupDestroyed(int groupID);

		/** The toString() function */
		friend std::ostream& operator<<(std::ostream&, rcAttackTask);
	};

	void Init();

	/**
	 * Add a build task to the taskholder
	 * 
	 * @param Group*, the group who will be performing the task
	 * @param buildTaskType, the abstract buildtask type
	 * @param UnitType*, the unit to build
	 * @param float3, the position where to build
	 */
	void AddBuildTask(pAIGroup, BuildTask::BuildTaskType, pcUnitDef, rFloat3);

	/**
	 * Add an attack task to the taskholder
	 *
	 * @param Group*, the group who will perform the task
	 * @param int, the target to attack
	 */
	void AddAttackTask(pAIGroup, int);

	void AddAssistAttackTask(pAIGroup group, pAttackTask task);
	void AddAssistBuildTask(pAIGroup group, pBuildTask task);

	/**
	 * Get the closest specific buildtask
	 *
	 * @param buildTaskType, the type of buildtask
	 * @param float3&, the position
	 */
	pBuildTask GetClosestBuildTask(BuildTask::BuildTaskType, rcFloat3);

	/**
	 * Get the targets that are in the attack list
	 *
	 * @param std::list<int>&, the list that will be holding the targets
	 */
	void GetAttackingTargets(rvInt);

private:

	/** Build tasks with groupid as key, also includes assisters */
	std::map<Uint32, pBuildTask> mBuildTasks;

	/** Attack tasks with groupid as key, also include assisters */
	std::map<Uint32, pAttackTask> mAttackTasks;

	// Implementation
	void GroupDestroyed(int groupID);
};

#endif
