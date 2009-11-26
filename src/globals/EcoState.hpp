#ifndef ECO_STATE
#define ECO_STATE

#include <list>

#define HISTORY 10

class UnitType;
class Group;
class AIHelper;

class EcoState {
	public:
		EcoState(){}
		~EcoState(){}

		/**
		 * The semi constructor, sets reference to IAICallback
		 *
		 * @param AIHelper, the global instances holder
		 * over which is averaged
		 */
		void Init(AIHelper*);

		/**
		 * The Update() function calculates the current economic state
		 */
		void Update();

		/**
		 * Determine whether a factory can be assisted
		 *
		 * @param Group*, the group that can potentially assist
		 * @return Group*, the factory which to assist
		 */
		Group* CanAssistFactory(Group*);

		/**
		 * Determine whether a certain unit can be build
		 *
		 * @param Group*, the group that performs the potential build
		 * @param UnitType*, the unit to be build
		 * @return bool
		 */
		bool CanAffordToBuild(Group*, UnitType*);

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


		std::list<Group*> factories;
		std::list<Group*> workers;
		std::list<Group*> nanotowers;
		std::list<Group*> metalextractors;
		std::list<Group*> metalmakers;

		AIHelper *aih;
};

#endif
