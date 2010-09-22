#ifndef GROUP_HDR
#define GROUP_HDR

#include <list>
#include <stack>
#include <map>

#include "../main/Types.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

DECLARE_CLASS(AIUnit)
DECLARE_CLASS(LuaModule)

class AIGroup: public AUnitDestroyedObserver {
public:
	AIGroup(): gid(sCounter) { sCounter++; }
	~AIGroup(){}

	void AddUnit(pAIUnit);
	void Update(); // called by engine update()
	void Release(); // Release the group

	void AddModule(pLuaModule);
	void RemoveModule(pLuaModule);
	void PushModule(pLuaModule);

	float3 GetPos();
	int GetId() { return gid; }
	
private:
	static int sCounter;
	int gid;

	std::map<int, pAIUnit> units;
	std::list<pLuaModule> modules;
	std::stack<pLuaModule> moduleStack;

	// implementation
	void UnitDestroyed(int unit);
};

#endif
