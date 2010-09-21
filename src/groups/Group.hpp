#ifndef GROUP_HDR
#define GROUP_HDR

#include <list>
#include <stack>
#include <map>

#include "../main/Types.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

DECLARE_CLASS(AModule)
DECLARE_CLASS(AIUnit)

class Group: public AUnitDestroyedObserver {
	public:
		Group(): gid(sCounter) { sCounter++; }
		~Group(){}

		void AddUnit(pAIUnit);
		void Update(); // called by engine update()
		void Release(); // Release the group

		void AddModule(pAModule);
		void RemoveModule(pAModule);
		void PushModule(pAModule);

		float3 GetPos();
		int GetId() { return gid; }
		
	private:
		static int sCounter;
		int gid;

		std::map<int, pAIUnit> units;
		std::list<pAModule> modules;
		std::stack<pAModule> moduleStack;

		// implementation
		void UnitDestroyed(int unit);
};

#endif
