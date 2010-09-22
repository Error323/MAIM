#ifndef AI_UNIT_HANDLER_HDR
#define AI_UNIT_HANDLER_HDR

#include "../main/Types.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"

DECLARE_CLASS(AIUnit)

class AIUnitHandler: public AUnitDestroyedObserver {
public:
	AIUnitHandler() {}
	~AIUnitHandler() {}

	void AddUnit(pAIUnit);
	pAIUnit GetUnit(int unitID);

	// override AUnitDestroyedObserver::UnitDestroyed
	void UnitDestroyed(int unitID);

private:
	std::map<int, pAIUnit> mUnits;

};

#endif
