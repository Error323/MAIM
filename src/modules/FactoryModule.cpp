#include "./FactoryModule.hpp"

#include "../main/AIHelper.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../globals/EcoState.hpp"
#include "../factories/ReusableObjectFactory.hpp"

void FactoryModule::Init(AIHelper* aih) {
	this->aih = aih;

	// Module actives on these unit types:
	moduleTypeMasks = MASK_BUILDER_STATIC;
	moduleTerrainMasks = 0;
	moduleWeaponMasks = 0;
	moduleMoveMasks = 0;
}

void FactoryModule::Release() {
	ReusableObjectFactory<FactoryModule>::Release(this);
}

void FactoryModule::Filter(std::map<int, AIUnit*> &allunits) {
	std::map<int, AIUnit*>::iterator i;
	for (i = allunits.begin(); i != allunits.end(); i++) {
		const AIUnitDef* aud = i->second->GetUnitDef();

		if (IsSuited(aud->typeMask, aud->terrainMask, aud->weaponMask, aud->boMoveDataMask))
			units[i->first] = i->second;
	}
}

bool FactoryModule::CanRun() {
	bool stalling = aih->ecostate->IsStallingMetal() || aih->ecostate->IsStallingEnergy();
	if (!waiting.empty() && !stalling)
		return true;

	if (waiting.empty() && stalling)
		return true;

	std::map<int, AIUnit*>::iterator i;
	for (i = units.begin(); i != units.end(); i++)
		if (i->second->GetCommandQueueSize() <= MIN_QUEUE)
			return true;

	return false;
}

bool FactoryModule::Update() {
	// if we are stalling, put a wait on the factories
	// else make sure factories aren't waiting
	// foreach factory that has a command queue <= MIN_QUEUE, fill it up to MAX_QUEUE
	
	// We are always done when all factory queues are filled up again
	return true;
}
