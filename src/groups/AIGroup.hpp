#ifndef GROUP_HDR
#define GROUP_HDR

#include <list>
#include <stack>
#include <map>

#include "../main/Types.hpp"
#include "../observers/AGroupDestroyedSubject.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIUnit)
DECLARE_CLASS(LuaModule)

class AIGroup: public AGroupDestroyedSubject, AUnitDestroyedObserver {
public:
	AIGroup(): gid(sCounter) { SetGroupDestroyedSubjectID(gid); sCounter++; }
	~AIGroup(){}

	void  AddUnit(pAIUnit, cBool isNewGroup);
	void  Update(); // called by engine update()
	void  Release(); // Release the group
	cBool CanBeAdded(pAIUnit) const;

	float3 GetPos();
	int GetID() { return gid; }
	
private:
	static int sCounter;
	cInt gid;

	std::map<int, pAIUnit> units;
	std::list<pLuaModule> modules;
	std::stack<pLuaModule> moduleStack;

	// implementation
	void UnitDestroyed(int unit);

	void AddModule(pLuaModule);
	void PushModule(pLuaModule);
};

#endif
