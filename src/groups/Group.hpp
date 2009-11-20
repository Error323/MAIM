#ifndef GROUP
#define GROUP

#include <list>
#include <stack>

#include "../observers/AObserver.hpp"

class UnitDestroyedObserver;
class ASubject;
class AModule;

class Group: AObserver {
	public:
		Group(UnitDestroyedObserver *_ud): unitDestroyed(_ud){}
		~Group();

		void AddUnit(int unit);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule *module); // ModuleHolder override
		void RemoveModule(AModule *module); // ModuleHolder override
		
	private:
		std::list<int> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;
		UnitDestroyedObserver *unitDestroyed;

		void Update(ASubject *subject);
};

#endif
