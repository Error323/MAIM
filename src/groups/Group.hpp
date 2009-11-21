#ifndef GROUP
#define GROUP

#include <list>
#include <stack>

#include "../observers/AUnitDestroyedObserver.hpp"

class AModule;

class Group: public AUnitDestroyedObserver {
	public:
		Group(){}
		~Group(){}

		void AddUnit(int unit);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule *module);
		void RemoveModule(AModule *module);
		
	private:
		std::list<int> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
