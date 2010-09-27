#ifndef AI_ECO_STATE_HDR
#define AI_ECO_STATE_HDR

#include <list>
#include "../main/AITypes.hpp"
#include "../units/AIUnitDef.hpp"

#define HISTORY 10

DECLARE_CLASS(AIUnit)
DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIHelper)

DECLARE_CLASS(EcoState)

class EcoState {
	public:
		EcoState() {}
		~EcoState() {}

		void Init();

		/**
		 * The Update() function calculates the current economic state
		 */
		void Update();

		/**
		 * Determine whether a factory can be assisted
		 *
		 * @param pGroup, the group that can potentially assist
		 * @return pGroup, the factory which to assist
		 */
		bool CanAssistFactory(pcAIGroup);

		/**
		 * Get the best buildable unit with the includes/excludes specifications
		 * `best` being the most expensive one we can currently afford
		 * 
		 * @param pcAIUnit, the unit that will perform the construction
		 * @param rcAIUnitDefClass, the included masks
		 * @param rcAIUnitDefClass, the excluded masks
		 * @return the build id (a negative number)
		 */
		// TODO: Move to lua
		int GetBuildUnitID(pcAIUnit, const AIUnitDef::AIUnitDefClass&, const AIUnitDef::AIUnitDefClass&);

		/**
		 * Determine whether a certain unit can be build
		 *
		 * @param pAIUnitDef, the unit to be build
		 * @return bool
		 */
		bool CanAffordToBuild(pcAIUnit, pcAIUnitDef);

		bool IsStallingMetal() { return mStalling; }
		bool IsStallingEnergy() { return eStalling; }
		bool IsExceedingMetal() { return mExceeding; }
		bool IsExceedingEnergy() { return eExceeding; }

		float GetMetalNow() { return mNow; }
		float GetEnergyNow() { return eNow; }
		float GetMetalStorage() { return mStorage; }
		float GetEnergyStorage() { return eStorage; }
		float GetMetalIncome() { return mIncome; }
		float GetEnergyIncome() { return eIncome; }
		float GetMetalUsage() { return mUsage; }
		float GetEnergyUsage() { return eUsage; }

	private:
		std::list<float> histMNow, histENow;
		std::list<float> histMIncome, histEIncome;
		std::list<float> histMUsage, histEUsage;
		std::list<float> weights;

		float mStorage, eStorage;
		float mNow, eNow;
		float mIncome, eIncome;
		float mUsage, eUsage;

		bool mStalling, eStalling;
		bool mExceeding, eExceeding;


		std::list<pAIGroup> factories;
		std::list<pAIGroup> workers;
		std::list<pAIGroup> nanotowers;
		std::list<pAIGroup> metalextractors;
		std::list<pAIGroup> metalmakers;
};

#endif // ECO_STATE
