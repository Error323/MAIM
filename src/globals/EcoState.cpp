#include "EcoState.hpp"

#include "../main/HAIInterface.hpp"
#include "../main/AIHelper.hpp"
#include "../utils/Util.hpp"
#include "../utils/Logger.hpp"
#include "../groups/Group.hpp"

void EcoState::Init(pAIHelper aih) {
	this->aih = aih;

	// Create a Gaussian distribution over the HISTORY
	float sumw = 0.0f;
	for (int i = 0; i < HISTORY; i++) 
	{
		float w = util::GaussDens(float(i), 0.0f, 5.0f);
		sumw += w;
		weights.push_back(w);
	}

	// Normalize the weights
	std::list<float>::iterator w;
	for (w = weights.begin(); w != weights.end(); w++) 
	{
		*w /= sumw;
	}

	// Make sure we don't get bad values at the start
	for (int i = 0; i < HISTORY; i++)
	{
		Update();
	}

}

void EcoState::Update() {
	while (histMNow.size() > HISTORY) 
	{
		histMNow.pop_back();
		histENow.pop_back();
		histMIncome.pop_back();
		histEIncome.pop_back();
		histMUsage.pop_back();
		histEUsage.pop_back();
	}

	histMNow.push_front(aih->rcb->GetMetal());
	histENow.push_front(aih->rcb->GetEnergy());
	histMIncome.push_front(aih->rcb->GetMetalIncome());
	histEIncome.push_front(aih->rcb->GetEnergyIncome());
	histMUsage.push_front(aih->rcb->GetMetalUsage());
	histEUsage.push_front(aih->rcb->GetEnergyUsage());

	mNow = util::WeightedAverage(histMNow, weights);
	eNow = util::WeightedAverage(histENow, weights);
	mIncome = util::WeightedAverage(histMIncome, weights);
	eIncome = util::WeightedAverage(histEIncome, weights);
	mUsage = util::WeightedAverage(histMUsage, weights);
	eUsage = util::WeightedAverage(histEUsage, weights);

	mStorage = aih->rcb->GetMetalStorage();
	eStorage = aih->rcb->GetEnergyStorage();

	mStalling = (mNow/mStorage) < 0.1f && mIncome < mUsage;
	eStalling = (eNow/eStorage) < 0.1f && eIncome < eUsage;

	mExceeding = (mNow/mStorage) > 0.9f && mIncome > mUsage;
	eExceeding = (eNow/eStorage) > 0.9f && eIncome > eUsage;
}

pGroup EcoState::CanAssistFactory(pGroup) {
	return NULL;
}

bool EcoState::CanAffordToBuild(pGroup, pUnitType) {
	return true;
}
