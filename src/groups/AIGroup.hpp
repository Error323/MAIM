#ifndef AI_GROUP_HDR
#define AI_GROUP_HDR

#include <list>
#include <stack>
#include <map>

#include "../main/AITypes.hpp"
#include "../observers/AGroupDestroyedSubject.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"
#include "System/float3.h"

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIUnit)
DECLARE_CLASS(LuaModule)

class AIGroup: public AGroupDestroyedSubject, AUnitDestroyedObserver {
public:
	AIGroup();
	~AIGroup() {}

	bool CanAddUnit(pAIUnit) const;
	void AddUnit(pAIUnit, bool isNewGroup);

	void Update(); // called by engine update()
	void Release(); // Release the group

	float3 GetPos() const;
	unsigned int GetID() const { return groupID; }
	const std::map<int, pAIUnit>& GetUnits() { return units; }
	
	friend std::ostream& operator << (std::ostream&, rcAIGroup);

private:
	static unsigned int sGroupCounter;
	const unsigned int groupID;

	std::map<int, pAIUnit> units;
	vpLuaModule modules;
	pLuaModule activeModule;

	// implementation
	void UnitDestroyed(unsigned int unitID);

	void AddModule(pLuaModule);
	void PushModule(pLuaModule);
};

#endif
