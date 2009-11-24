#include "AModule.hpp"

#include "../groups/Group.hpp"
#include "../observers/AUnitDestroyedObserver.hpp"
#include "../utils/Util.hpp"

void AModule::SetGroup(Group *group) {
	this->group = group;
}

bool AModule::IsSuited(unsigned unitTypeMasks, unsigned unitTerrainMasks, unsigned unitWeaponMasks, unsigned unitMoveMasks) {
	bool a = util::IsBinarySubset(moduleTypeMasks, unitTypeMasks);
	bool b = util::IsBinarySubset(moduleTerrainMasks, unitTerrainMasks);
	bool c = util::IsBinarySubset(moduleWeaponMasks, unitWeaponMasks);
	bool d = util::IsBinarySubset(moduleMoveMasks, unitMoveMasks);
	return (a && b && c && d);
}

void AModule::UnitDestroyed(int unit) {
	units.erase(unit);
}
