#include "EcoState.hpp"

#include "../main/HAIInterface.hpp"
#include "../main/AIHelper.hpp"
#include "../groups/AIGroup.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/Util.hpp"
#include "../utils/Logger.hpp"

void EcoState::Init() {
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
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();
	
	while (histMNow.size() > HISTORY) 
	{
		histMNow.pop_back();
		histENow.pop_back();
		histMIncome.pop_back();
		histEIncome.pop_back();
		histMUsage.pop_back();
		histEUsage.pop_back();
	}

	histMNow.push_front(rcb->GetMetal());
	histENow.push_front(rcb->GetEnergy());
	histMIncome.push_front(rcb->GetMetalIncome());
	histEIncome.push_front(rcb->GetEnergyIncome());
	histMUsage.push_front(rcb->GetMetalUsage());
	histEUsage.push_front(rcb->GetEnergyUsage());

	mNow = util::WeightedAverage(histMNow, weights);
	eNow = util::WeightedAverage(histENow, weights);
	mIncome = util::WeightedAverage(histMIncome, weights);
	eIncome = util::WeightedAverage(histEIncome, weights);
	mUsage = util::WeightedAverage(histMUsage, weights);
	eUsage = util::WeightedAverage(histEUsage, weights);

	mStorage = rcb->GetMetalStorage();
	eStorage = rcb->GetEnergyStorage();

	mStalling = (mNow/mStorage) < 0.1f && mIncome < mUsage;
	eStalling = (eNow/eStorage) < 0.1f && eIncome < eUsage;

	mExceeding = (mNow/mStorage) > 0.9f && mIncome > mUsage;
	eExceeding = (eNow/eStorage) > 0.9f && eIncome > eUsage;
}

bool EcoState::CanAssistFactory(pcAIGroup aiGroup) {
	return true;
}

bool EcoState::CanAffordToBuild(pcAIUnit aiUnit, pcAIUnitDef aiUnitDef) {
	cFloat buildTime = aiUnitDef->GetDef()->buildTime / aiUnit->GetUnitDef()->GetDef()->buildSpeed;
	cFloat mPrediction = mNow + (mIncome - mUsage) * buildTime - aiUnitDef->GetDef()->metalCost;
	cFloat ePrediction = eNow + (eIncome - eUsage) * buildTime - aiUnitDef->GetDef()->energyCost;

	return (mPrediction >= 0.0f && ePrediction >= 0.0f);
}
