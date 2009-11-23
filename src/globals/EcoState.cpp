#include "EcoState.hpp"

#include "../main/AIHelper.hpp"
#include "../utils/Util.hpp"
#include "../groups/Group.hpp"

void EcoState::Init(AIHelper* aih) {
	this->aih = aih;
	Update();
}

void EcoState::Update() {
	if (histMNow.size() > 10) {
		histMNow.pop_front();
		histENow.pop_front();
		histMIncome.pop_front();
		histEIncome.pop_front();
		histMUsage.pop_front();
		histEUsage.pop_front();
	}

	histMNow.push_back(aih->rcb->GetMetal());
	histENow.push_back(aih->rcb->GetEnergy());
	histMIncome.push_back(aih->rcb->GetMetalIncome());
	histEIncome.push_back(aih->rcb->GetEnergyIncome());
	histMUsage.push_back(aih->rcb->GetMetalUsage());
	histEUsage.push_back(aih->rcb->GetEnergyUsage());

	mNow = util::average(histMNow);
	eNow = util::average(histENow);
	mIncome = util::average(histMIncome);
	eIncome = util::average(histEIncome);
	mUsage = util::average(histMUsage);
	eUsage = util::average(histEUsage);
	mStorage = aih->rcb->GetMetalStorage();
	eStorage = aih->rcb->GetEnergyStorage();

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
