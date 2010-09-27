#include "LegacyCpp/IAICallback.h"

#include "./EcoState.hpp"
#include "../main/AIHelper.hpp"
#include "../groups/AIGroup.hpp"
#include "../units/AIUnit.hpp"
#include "../units/AIUnitDefHandler.hpp"
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

int EcoState::GetBuildUnitID(pcAIUnit aiUnit, const AIUnitDef::AIUnitDefClass& includes, const AIUnitDef::AIUnitDefClass& excludes) {
	pAIHelper aih           = AIHelper::GetActiveInstance();
	rcvpcAIUnitDef unitDefs = aih->GetAIUnitDefHandler()->GetUnitDefs();
	pcAIUnitDef bestUnitDef = NULL;
	float bestCost          = 0.0f;

	// loop through aiunitdefs and select the one that can be afforded to build
	for (Uint32 i = 0; i < unitDefs.size(); i++)
	{
		// If the unit can be build by aiUnit
		if (util::AreSuitedSubjects(unitDefs[i]->unitDefClass, includes, excludes))
		{
			// If we can afford it with our current economic state
			if (CanAffordToBuild(aiUnit, unitDefs[i]))
			{
				// TODO: Metal2Energy conversion
				cFloat cost = unitDefs[i]->GetDef()->metalCost + unitDefs[i]->GetDef()->energyCost;

				// Assuming more expensive unit is better
				if (cost > bestCost)
				{
					bestCost = cost;
					bestUnitDef = unitDefs[i];
				}
			}
		}
	}
	
	if (bestUnitDef == NULL)
	{
		LOG_ERROR("[EcoState::GetBuildUnitID] " << *aiUnit << " can't build unit specified by includes: " << includes << ", excludes: " << excludes << "\n");
		return 0; // Doesn't exist
	}
	else
	{
		return -bestUnitDef->GetID(); // Negative ID is used for building
	}
}

bool EcoState::CanAffordToBuild(pcAIUnit aiUnit, pcAIUnitDef aiUnitDef) {
	cFloat buildTime = aiUnitDef->GetDef()->buildTime / aiUnit->GetUnitDef()->GetDef()->buildSpeed;
	cFloat mPrediction = mNow + (mIncome - mUsage) * buildTime - aiUnitDef->GetDef()->metalCost;
	cFloat ePrediction = eNow + (eIncome - eUsage) * buildTime - aiUnitDef->GetDef()->energyCost;

	return (mPrediction >= 0.0f && ePrediction >= 0.0f);
}
