#ifndef GROUP
#define GROUP

#include <list>
#include <stack>

#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

class AModule;

class Group: public AUnitDestroyedObserver {
	public:
		Group(){}
		~Group(){}

		void AddUnit(int);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule*);
		void RemoveModule(AModule*);
		void PushModule(AModule*);

		float3 GetPos();
		
	private:
		std::list<int> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
