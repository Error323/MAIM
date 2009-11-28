#ifndef GROUP_HDR
#define GROUP_HDR

#include <list>
#include <stack>
#include <map>

#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

class AModule;
class AIUnit;

class Group: public AUnitDestroyedObserver {
	public:
		Group(){ gid(counter++); }
		~Group(){}

		void AddUnit(AIUnit*);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(AModule*);
		void RemoveModule(AModule*);
		void PushModule(AModule*);

		float3 GetPos();
		int GetId() { return gid; }
		
	private:
		static int counter;
		int gid;

		std::map<int, AIUnit*> units;
		std::list<AModule*> modules;
		std::stack<AModule*> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
