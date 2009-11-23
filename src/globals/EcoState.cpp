#include "EcoState.hpp"

#include "../utils/Util.hpp"
#include "../groups/Group.hpp"

void EcoState::Init(IAICallback *cb, int history) {
	this->cb = cb;
	this->history = history;
	Update();
}

void EcoState::Update() {
	if (histMNow.size() > history) {
		histMNow.pop_front();
		histENow.pop_front();
		histMIncome.pop_front();
		histEIncome.pop_front();
		histMUsage.pop_front();
		histEUsage.pop_front();
	}

	histMNow.push_back(cb->GetMetal());
	histENow.push_back(cb->GetEnergy());
	histMIncome.push_back(cb->GetMetalIncome());
	histEIncome.push_back(cb->GetEnergyIncome());
	histMUsage.push_back(cb->GetMetalUsage());
	histEUsage.push_back(cb->GetEnergyUsage());

	mNow = util::average(histMNow);
	eNow = util::average(histENow);
	mIncome = util::average(histMIncome);
	eIncome = util::average(histEIncome);
	mUsage = util::average(histMUsage);
	eUsage = util::average(histEUsage);
	mStorage = cb->GetMetalStorage();
	eStorage = cb->GetEnergyStorage();

	mStalling = (mNow/mStorage) < 0.1f && mIncome < mUsage;
	eStalling = (eNow/eStorage) < 0.1f && eIncome < eUsage;

	mExceeding = (mNow/mStorage) > 0.9f && mIncome > mUsage;
	eExceeding = (eNow/eStorage) > 0.9f && eIncome > eUsage;
}

Group* EcoState::CanAssistFactory(Group*) {
	return NULL;
}

bool EcoState::CanAffordToBuild(Group*, UnitType*) {
	return true;
}
