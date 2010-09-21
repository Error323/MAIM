#include "AIUnitDef.hpp"

#include "../factories/Factory.hpp"
#include "../lua/AILuaModule.hpp"

// Include all concrete modules here

std::list<LuaModule*> AIUnitDef::allmodules;
// Add all modules to allmodules list here, example:
// AIUnitDef::allmodules.push_back(ReusableObjectFactory<XXX>::Instance());

void AIUnitDef::CalcModules() {
	// Initialize all module here, add those that fit the masks
	std::list<LuaModule*>::iterator i;
	for (i = AIUnitDef::allmodules.begin(); i != AIUnitDef::allmodules.end(); i++) {
		if ((*i)->IsSuited(typeMask, terrainMask, weaponMask, boMoveDataMask))
			modules.push_back(*i);
	}
}
