#ifndef TASK_HOLDER
#define TASK_HOLDER

#include <list>
#include <map>

enum taskType {
	BUILD_FACTORY, 
	BUILD_DEFENSE, 
	BUILD_ENERGY, 
	BUILD_METAL,
	SCOUT,
	ATTACK
};

class Group;

class TaskHolder {
	public:
		TaskHolder(){}
		~TaskHolder(){}

		void Init();

		/**
		 * Add a task to the taskholder
		 * 
		 * @param taskType, the type of task
		 * @param Group*, the group who will be performing it
		 */
		void AddTask(taskType, Group*);

		/**
		 * Get a list of groups performing a task of taskType
		 *
		 * @param taskType, the type of task
		 * @return std::list<Group*>&, the list of groups
		 */
		std::list<Group*>& GetTasks(taskType);

		/**
		 * Query whether a certain task of taskType is being performed
		 *
		 * @param taskType, the type of task
		 * @return bool
		 */
		bool IsTaskBeingPerformed(taskType);

		/**
		 * Query for the closest task of taskType
		 *
		 * @param taskType, the type of task
		 * @return Group*, the group performing it
		 */
		Group* GetClosestTask(taskType);

	private:
		/** <taskType, groups performing a taskType task> */
		std::map<taskType, std::list<Group*> > tasks;

		/** <Main group, Assister groups> */
		std::map<Group*, Group*> assisters;
};

#endif
