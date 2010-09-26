#ifndef AI_UNIT_HANDLER_HDR
#define AI_UNIT_HANDLER_HDR

#include <map>

#include "../main/AITypes.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"

DECLARE_CLASS(AIUnit)

class AIUnitHandler: public AUnitDestroyedObserver {
public:
	AIUnitHandler() {}
	~AIUnitHandler() {}

	void UnitCreated(pAIUnit);
	// override AUnitDestroyedObserver::UnitDestroyed
	void UnitDestroyed(unsigned int unitID);

	pAIUnit GetUnit(unsigned int unitID);

private:
	std::map<unsigned int, pAIUnit> mUnits;

};

#endif
