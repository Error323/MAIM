#ifndef GAME_MAP
#define GAME_MAP

#include <list>

#include "System/float3.h"

// heightVariance(Altair_Crossing.smf)
#define KBOT_VEH_THRESHOLD 43.97f

class Group;
class AIHelper;

class GameMap {
	public:
		GameMap(){}
		~GameMap(){}

		void Init(AIHelper*);

		/** @return float3, ZeroVector when there are no candidates */
		float3 GetClosestOpenMetalSpot(Group*);

		/** @return int, unit id of the upgradeable mex, -1 if there are no candidates */
		int GetClosestUpgradableMetalSpot(Group*);

		/** @return float, height variance */
		float GetHeightVariance() { return heightVariance; }

		/** @return float, amount of water in [0, 1] */
		float GetAmountOfWater() { return waterAmount; }

		/** @return float, amount of land in [0, 1] */
		float GetAmountOfLand() { return 1.0f-waterAmount; }


		bool HasMetalSpots() { return metalAmount > 0.0f; }
		bool HasGeoSpots() { return geospots.size() > 0; }
		bool HasMetalFeatures() { return metalfeatures.size() > 0; }
		bool HasEnergyFeatures() { return energyfeatures.size() > 0; }

		bool IsKbotMap() { return heightVariance > KBOT_VEH_THRESHOLD; }
		bool IsVehicleMap() { return !IsKbotMap(); }
		bool IsMetalMap() { return metalAmount > 0.0f; }

		std::list<float3>& GetGeoSpots() { return geospots; }
		std::list<float3>& GetMetalFeatures() { return metalfeatures; }
		std::list<float3>& GetEnergyFeatures() { return energyfeatures; }
	
	private:
		float heightVariance;
		float waterAmount;
		float metalAmount;

		std::list<float3> geospots;
		std::list<float3> metalfeatures;
		std::list<float3> energyfeatures;
		std::list<float3> metalspots;

		AIHelper *aih;

		void CalcMetalSpots();
		void CalcMapHeightFeatures();
		void CalcGeoSpots();
};

#endif
