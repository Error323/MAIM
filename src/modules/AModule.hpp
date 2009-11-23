#ifndef AMODULE
#define AMODULE

#include <list>
#include <stack>
#include <string>

#include "../observers/AUnitDestroyedObserver.hpp"

class Group;

class AModule: public AUnitDestroyedObserver {
	public:
		virtual ~AModule();

		virtual void Release() = 0;
		virtual void Filter(std::list<int> &units) = 0;
		virtual bool Update() = 0;
		virtual bool CanRun() = 0;

		void SetGroup(Group *group);

		unsigned categories;

	protected:
		AModule();
		Group *group; // the group to which the module belongs
		std::list<int> units; // subset of group units
		std::string name; // name of this module

	private:
		// Implementation
		void UnitDestroyed(int unit);
};

#endif
