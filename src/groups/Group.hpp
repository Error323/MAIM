#ifndef GROUP
#define GROUP

#include <list>
#include <stack>

#include "../observers/AUnitDestroyedObserver.hpp"

class AModule;

class Group: AUnitDestroyedObserver {
	public:
		Group(){}
		~Group(){}

		void AddUnit(int unit);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule *module); // ModuleHolder override
		void RemoveModule(AModule *module); // ModuleHolder override
		
	private:
		std::list<int> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
