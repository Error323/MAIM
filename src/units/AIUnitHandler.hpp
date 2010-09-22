#ifndef AI_UNIT_HANDLER_HDR
#define AI_UNIT_HANDLER_HDR

#include "../main/Types.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"

DECLARE_CLASS(AIUnit)

class AIUnitHandler: public AUnitDestroyedObserver {
public:
	AIUnitHandler() {}
	~AIUnitHandler() {}

	void UnitCreated(pAIUnit);
	// override AUnitDestroyedObserver::UnitDestroyed
	void UnitDestroyed(int unitID);

	pAIUnit GetUnit(int unitID);

private:
	std::map<int, pAIUnit> mUnits;

};

#endif
