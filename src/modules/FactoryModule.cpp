#include "./FactoryModule.hpp"

#include "../main/AIHelper.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../factories/Factory.hpp"

void FactoryModule::Init(AIHelper* h) {
	aih = h;

	// Module actives on these unit types:
	moduleTypeMasks = MASK_BUILDER_STATIC;
	moduleTerrainMasks = 0;
	moduleWeaponMasks = 0;
	moduleMoveMasks = 0;
}

void FactoryModule::Release() {
	Factory<FactoryModule>::Release(this);
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
	std::map<int, AIUnit*>::iterator i;
	for (i = units.begin(); i != units.end(); i++)
		if (i->second->GetCommandQueueSize() <= MIN_QUEUE)
			return true;

	return false;
}

bool FactoryModule::Update() {
	// Fill up the factories that have a queue <= MIN_QUEUE
	std::map<int, AIUnit*>::iterator i;
	for (i = units.begin(); i != units.end(); i++) {
		int queueSize = i->second->GetCommandQueueSize();
		if (queueSize <= MIN_QUEUE) {
			for (int j = 0; j < (MAX_QUEUE-queueSize); j++) {
				// First see if we have enough eco workers, if not
				// build an eco worker
				//
				// i->second->Build(WORKER, bool enqueue = true);
				//
				// Else, using globals/Intel.hpp we determine the offensive to build
				// i->second->Build(aih->intel->GetOffensive(), bool enqueue = true);
			}
		}
	}
	
	// We are always done when all factory queues are filled up again
	return true;
}
