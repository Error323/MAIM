#ifndef TASK_HOLDER
#define TASK_HOLDER

#include <list>
#include <map>
#include <iostream>

#include "ExternalAI/IAICallback.h"

enum taskType { BUILD, ATTACK };
enum buildTaskType { FACTORY, METAL, ENERGY, DEFENSE };

class Group;
class AttackTask;
class BuildTask;

class TaskHolder {
	public:
		TaskHolder(){}
		~TaskHolder(){}

		void Init(IAICallback*);

		/**
		 * Add a build task to the taskholder
		 * 
		 * @param Group*, the group who will be performing the task
		 * @param buildTaskType, the abstract buildtask type
		 * @param UnitType*, the unit to build
		 * @param float3, the position where to build
		 */
		void AddBuildTask(Group*, buildTaskType, UnitType*, float3);

		/**
		 * Add an attack task to the taskholder
		 *
		 * @param Group*, the group who will perform the task
		 * @param int, the target to attack
		 */
		void AddAttackTask(Group*, int);

		/**
		 * Get a list of build tasks
		 *
		 * @return std::list<BuildTask*>&, the list of buildtasks
		 */
		std::list<BuildTask*>& GetBuildTasks();

		/**
		 * Get a list of attack tasks
		 *
		 * @return std::list<AttackTask*>&, the list of attacktasks
		 */
		std::list<AttackTask*>& GetAttackTasks();

		/**
		 * Get the closest specific buildtask
		 *
		 * @param buildTaskType, the type of buildtask
		 * @param float3&, the position
		 */
		BuildTask* GetClosestBuildTask(buildTaskType, float3&);

		/**
		 * Get the targets that are in the attack list
		 *
		 * @param std::list<int>&, the list that will be holding the targets
		 */
		void GetAttackingTargets(std::list<int>&);

	private:
		/** Build tasks */
		std::list<BuildTask*> buildtasks;

		/** Attack tasks */
		std::list<AttackTask*> attacktasks;
};

struct BuildTask {
	BuildTask(){}

	/** The abstract build */
	buildTaskType btt;

	/** The estimated time of this build (inc walktime) */
	unsigned eta;

	/** The actual timer of the build, updated in Group::Update() */
	unsigned timer;

	/** The unit to build */
	UnitType *build;

	/** The primary group that applies the build */
	Group *alpha;

	/** The assisters of the build */
	std::list<Group*> assisters;

	/** The position where the build will take place */
	float3 pos;

	/** IAICallback reference */
	IAICallback *cb;

	/** The toString() function */
	friend std::ostream& operator<<(std::ostream&, const BuildTask&);
};

struct AttackTask {
	AttackTask(){}

	/** The target to attack */
	int target;

	/** The primary attack group */
	Group *alpha;

	/** The assisters of the attack */
	std::list<Group*> assisters;

	/** IAICallback reference */
	IAICallback *cb;

	/** The toString() function */
	friend std::ostream& operator<<(std::ostream&, const AttackTask&);
};

#endif
