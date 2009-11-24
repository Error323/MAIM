#ifndef AMODULE
#define AMODULE

#include <list>
#include <stack>
#include <map>
#include <string>

#include "../observers/AUnitDestroyedObserver.hpp"

class Group;
class AIUnit;

class AModule: public AUnitDestroyedObserver {
	public:
		virtual ~AModule();

		virtual void Release() = 0;
		virtual void Filter(std::map<int,AIUnit*>&) = 0;
		virtual bool Update() = 0;
		virtual bool CanRun() = 0;

		void SetGroup(Group *group);
		bool IsSuited(unsigned, unsigned, unsigned, unsigned);

	protected:
		AModule();
		Group *group; // the group to which the module belongs
		std::map<int, AIUnit*> units; // subset of group units
		std::string name; // name of this module

		unsigned moduleTypeMasks;
		unsigned moduleTerrainMasks;
		unsigned moduleWeaponMasks;
		unsigned moduleMoveMasks;

	private:
		// Implementation
		void UnitDestroyed(int unit);
};

#endif
