#ifndef AI_UNIT_MANAGER_HDR
#define AI_UNIT_MANAGER_HDR

#include "../main/Types.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"

DECLARE_CLASS(AIUnit)

class AIUnitManager: public AUnitDestroyedObserver {
public:
	AIUnitManager() {}
	~AIUnitManager() {}

	void AddUnit(pAIUnit);
	pAIUnit GetUnit(int unitID);

private:
	std::map<int, pAIUnit> mUnits;

	// implementation
	void UnitDestroyed(int unit);
};

#endif // AI_UNIT_MANAGER_HDR
