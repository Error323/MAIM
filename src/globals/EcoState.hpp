#ifndef ECO_STATE
#define ECO_STATE

#include "ExternalAI/IAICallback.h"

class UnitType;
class Group;

class EcoState {
	public:
		EcoState(){}
		~EcoState(){}

		/**
		 * The semi constructor, sets reference to IAICallback
		 *
		 * @param IAICallback, the spring callback handle
		 */
		void Init(IAICallback*);

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

		bool IsStallingMetal();
		bool IsStallingEnergy();
		bool IsExceedingMetal();
		bool IsExceedingEnergy();

		float GetMetalNow();
		float GetEnergyNow();
		float GetMetalStorage();
		float GetEnergyStorage();
		float GetMetalIncome();
		float GetEnergyIncome();
		float GetMetalUsage();
		float GetEnergyUsage();

	private:
		float mNow, eNow;
		float mIncome, eIncome;
		float mUsage, eUsage;
		float mStorage, eStorage;

		bool mStalling, eStalling;
		bool mExceeding, eExceeding;

		std::list<Group*> factories;
		std::list<Group*> workers;
		std::list<Group*> nanotowers;
		std::list<Group*> metalextractors;
		std::list<Group*> metalmakers;

		/** Spring callback function handle */
		IAICallback *cb;
};

#endif
