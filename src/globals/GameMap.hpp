#ifndef GAME_MAP
#define GAME_MAP

#include <list>

#include "ExternalAI/IAICallback.h"
#include "System/float3.h"

class Group;

class GameMap {
	public:
		GameMap(){}
		~GameMap(){}

		void Init(IAICallback*);

		float3 GetClosestOpenMetalSpot(Group*);
		Group* GetClosestUpgradableMetalSpot(Group*);

		float GetHeightVariance();
		float GetAmountOfWater();
		float GetAmountOfLand();
		float GetAmountOfMetal();

		bool HasMetalSpots();
		bool HasGeoSpots();
		bool HasMetalFeatures();
		bool HasEnergyFeatures();

		std::list<float3>& GetGeoSpots();
		std::list<float3>& GetMetalFeatures();
		std::list<float3>& GetEnergyFeatures();
	
	private:
		float heightVariance;
		float waterAmount;
		float landAmount;
		float metalAmount;

		std::list<float3> geospots;
		std::list<float3> metalfeatures;
		std::list<float3> energyfeatures;
		std::map<int, Group*> takenmetalspots;
		std::map<int, Group*> upgradedmetalspots;
};

#endif
