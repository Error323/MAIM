#ifndef AI_GAME_MAP_HDR
#define AI_GAME_MAP_HDR

#include <list>

#include "System/float3.h"
#include "../main/AITypes.hpp"

// heightVariance(Altair_Crossing.smf)
#define KBOT_VEH_THRESHOLD 43.97f

DECLARE_CLASS(AIGroup)
DECLARE_CLASS(AIHelper)

DECLARE_CLASS(GameMap)

class GameMap {
	public:
		GameMap() {}
		~GameMap() {}

		void Init();

		/** @return float3, ZeroVector when there are no candidates */
		float3 GetClosestOpenMetalSpot(pAIGroup);

		/** @return int, unit id of the upgradeable mex, -1 if there are no candidates */
		int GetClosestUpgradableMetalSpot(pAIGroup);

		/** @return float, height variance */
		float GetHeightVariance() { return heightVariance; }

		/** @return float, amount of water in [0, 1] */
		float GetAmountOfWater() { return waterAmount; }

		/** @return float, amount of land in [0, 1] */
		float GetAmountOfLand() { return (1.0f - waterAmount); }


		bool HasMetalSpots() { return (!metalspots.empty()); }
		bool HasGeoSpots() { return (!geospots.empty()); }
		bool HasMetalFeatures() { return (!metalfeatures.empty()); }
		bool HasEnergyFeatures() { return (!energyfeatures.empty()); }

		bool IsKbotMap() { return heightVariance > KBOT_VEH_THRESHOLD; }
		bool IsVehicleMap() { return !IsKbotMap(); }

		std::list<float3>& GetGeoSpots() { return geospots; }
		std::list<float3>& GetMetalFeatures() { return metalfeatures; }
		std::list<float3>& GetEnergyFeatures() { return energyfeatures; }
	
	private:
		float heightVariance;
		float waterAmount;

		static std::list<float3> geospots;
		static std::list<float3> metalfeatures;
		static std::list<float3> energyfeatures;
		static std::list<float3> metalspots;

		void CalcMetalSpots();
		void CalcMapHeightFeatures();
		void CalcGeoSpots();
};

#endif
