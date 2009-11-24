#ifndef GROUP
#define GROUP

#include <list>
#include <stack>
#include <map>

#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

class AModule;
class AIUnit;

class Group: public AUnitDestroyedObserver {
	public:
		Group(){}
		~Group(){}

		void AddUnit(AIUnit*);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule*);
		void RemoveModule(AModule*);
		void PushModule(AModule*);

		float3 GetPos();
		
	private:
		std::map<int, AIUnit*> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
