#ifndef UNITDEF_HANDLER
#define UNITDEF_HANDLER

#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "../utils/ILogger.hpp"

struct AIHelper;
struct UnitDef;
struct AIUnitDef;

class AIUnitDefHandler: public ILogger {
public:
	AIUnitDefHandler();
	~AIUnitDefHandler();
	void WriteLog();

	// return a list of all the sets that this ID is a member of
	// note: cache this per UnitDef instance upon insertion?
	std::list< std::set<int>* > GetUnitDefIDSetsForID(int);

	// return a list of all sets corresponding to the given masks
	// example: GetUnitDefIDSetsForMask(MASK_BUILDER_MOBILE, MASK_LAND, 0)
	std::list<std::set<int>* > GetUnitDefIDSetsForMask(unsigned int, unsigned int, unsigned int);

	// return a set of all UnitDef integer IDs that match the given mask restraints
	std::set<int> GetUnitDefIDsForMask(unsigned int, unsigned int, unsigned int, bool);

	// return the number of sets this type was inserted in
	int InsertUnitDefByID(int i);
	void CategorizeUnitDefByID(int i);

	// const unsigned int GetUnitDefCount() const { return (aiUnitDefsByID.size() - 1); }
	const AIUnitDef* GetUnitDefByID(int i) const { return aiUnitDefsByID[i]; }

private:
	std::set<int> mobileBuilderUnitDefIDs;
	std::set<int> staticBuilderUnitDefIDs;
	std::set<int> mobileAssisterUnitDefIDs;
	std::set<int> staticAssisterUnitDefIDs;
	std::set<int> mobileEProducerUnitDefIDs;
	std::set<int> staticEProducerUnitDefIDs;
	std::set<int> mobileMProducerUnitDefIDs;
	std::set<int> staticMProducerUnitDefIDs;
	std::set<int> mobileEStorageUnitDefIDs;
	std::set<int> staticEStorageUnitDefIDs;
	std::set<int> mobileMStorageUnitDefIDs;
	std::set<int> staticMStorageUnitDefIDs;
	std::set<int> staticDefenseUnitDefIDs;
	std::set<int> mobileDefenseUnitDefIDs;
	std::set<int> staticOffenseUnitDefIDs;
	std::set<int> mobileOffenseUnitDefIDs;
	std::set<int> mobileIntelUnitDefIDs;
	std::set<int> staticIntelUnitDefIDs;

	std::set<int> landUnitDefIDs;
	std::set<int> surfaceWaterUnitDefIDs;
	std::set<int> submergedWaterUnitDefIDs;
	std::set<int> airUnitDefIDs;

	std::set<int> armedUnitDefIDs;
	std::set<int> nukeUnitDefIDs;
	std::set<int> antiNukeUnitDefIDs;
	std::set<int> shieldUnitDefIDs;

	// stores all the sets above
	std::vector<std::set<int>* > unitDefIDSets;

	std::vector<const UnitDef*> sprUnitDefsByID;
	std::vector<const AIUnitDef*> aiUnitDefsByID;
};

#endif
