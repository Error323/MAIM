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
	AIGroup();
	~AIGroup(){}

	void  AddUnit(pAIUnit, cBool isNewGroup);
	void  Update(); // called by engine update()
	void  Release(); // Release the group
	cBool CanBeAdded(pAIUnit) const;

	float3 GetPos();
	int GetID() { return groupID; }
	
	friend std::ostream& operator << (std::ostream&, rcAIGroup);

private:
	static int sGroupCounter;
	cInt groupID;

	std::map<int, pAIUnit> units;
	vpLuaModule modules;
	pLuaModule activeModule;

	// implementation
	void UnitDestroyed(int unitID);

	void AddModule(pLuaModule);
	void PushModule(pLuaModule);
};

#endif
