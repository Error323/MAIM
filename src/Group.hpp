#ifndef MAI_GROUP
#define MAI_GROUP

#include <list>
#include <stack>

#include "./modules/ModuleHolder.hpp"
#include "./observers/AObserver.hpp"

class UnitDestroyed;
class ASubject;

class Group: ModuleHolder, AObserver {
	public:
		Group(UnitDestroyed *_ud): unitDestroyed(_ud);
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
		UnitDestroyed *unitDestroyed;

		void Update(ASubject *subject);
};

#endif
