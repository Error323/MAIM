#include <map>
#include <limits>
#include <iostream>

#include "System/float3.h"
#include "LegacyCpp/IAICallback.h"
#include "LegacyCpp/UnitDef.h"

#include "./GameMap.hpp"
#include "../main/AIDefines.hpp"
#include "../main/AIHelper.hpp"
#include "../groups/AIGroup.hpp"
#include "../utils/Logger.hpp"

#define METAL_THRESHOLD 32

std::list<float3> GameMap::geospots;
std::list<float3> GameMap::metalfeatures;
std::list<float3> GameMap::energyfeatures;
std::list<float3> GameMap::metalspots;

void GameMap::Init() {
	heightVariance = 0.0f;
	waterAmount    = 0.0f;

	CalcMapHeightFeatures();
	CalcMetalSpots();

	LOG_BASIC("[GameMap::HasMetalSpots] " << HasMetalSpots() << "\n");
	LOG_BASIC("[GameMap::HasGeoSpots] " << HasGeoSpots() << "\n");
	LOG_BASIC("[GameMap::HasEnergyFeatures] " << HasEnergyFeatures() << "\n");
	LOG_BASIC("[GameMap::HasMetalFeatures] " << HasMetalFeatures() << "\n");
	LOG_BASIC("[GameMap::IsKbotMap] " << IsKbotMap() << "\n");
	LOG_BASIC("[GameMap::IsVehicleMap] " << IsVehicleMap() << "\n");
	LOG_BASIC("[GameMap::HeightVariance] " << GetHeightVariance() << "\n");
	LOG_BASIC("[GameMap::GetAmountOfWater] " << GetAmountOfWater() << "\n");
	LOG_BASIC("[GameMap::GetAmountOfLand] " << GetAmountOfLand() << "\n");
	LOG_BASIC("[GameMap::CalcMetalSpots] found " << metalspots.size() << " metalspots\n");
}

float3 GameMap::GetClosestOpenMetalSpot(pAIGroup group) {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	const float3& gpos = group->GetPos();

	std::map<float, float3> candidates;
	std::list<float3>::iterator i;

	for (i = metalspots.begin(); i != metalspots.end(); i++) 
	{
		// examine the 50 units nearest to spot <i>
		// if one of these is an extractor, the spot
		// is probably already taken
		int units[50];
		int numUnits = rcb->GetFriendlyUnits(units, *i, rcb->GetExtractorRadius(), 50);
		bool taken = false;

		for (int j = 0; j < numUnits; j++) 
		{
			if (rcb->GetUnitDef(units[j])->extractsMetal > 0.0f) 
			{
				taken = true;
				break;
			}
		}

		if (!taken) 
		{
			cFloat radius = (gpos - *i).Length2D();
			candidates[radius] = *i;
		}
	}

	return (candidates.empty()? ZeroVector: candidates.begin()->second);
}


void GameMap::CalcMetalSpots() {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	cUint32 HMX = rcb->GetMapWidth();
	cUint32 HMZ = rcb->GetMapHeight();
	// scale the height-map down by a factor *four*
	// (the metal-map does NOT have this resolution)
	cUint32 X = MAP_HEIGHT_2_METAL(HMX) >> 1;
	cUint32 Z = MAP_HEIGHT_2_METAL(HMZ) >> 1;
	// also convert the extractor radius from
	// world-resolution to our scaled-down space
	int R = MAP_WORLD_2_METAL(Uint32(rcb->GetExtractorRadius())) >> 1;

	cFloat minimum = 10.0f * M_PI * R * R;

	pcUint8 metalmapData = rcb->GetMetalMap();
	vUint8 metalmap(X * Z, 0);

	// Calculate circular stamp
	std::vector<int> circle;
	std::vector<float> sqrtCircle;

	for (int i = -R; i <= R; i++) {
		for (int j = -R; j <= R; j++) {
			cFloat r = sqrtf(i * i + j * j);

			if (r > R)
				continue;

			circle.push_back(i);
			circle.push_back(j);
			sqrtCircle.push_back(r);
		}
	}

	vInt M;

	avgMetal = 0;
	minMetal = std::numeric_limits<int>::max();
	maxMetal = std::numeric_limits<int>::min();

	// Copy metalmap to mutable metalmap
	for (Uint32 z = R; z < (Z - R); z++) {
		for (Uint32 x = R; x < (X - R); x++) {
			Uint8 m = 0;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					cUint32 idx = (z * 2 + i) * X * 2 + (x * 2 + j);

					if (metalmapData[idx] > 1)
					{
						m = std::max<int>(metalmapData[idx], m);
					}
				}
			}

			if (m > 1) 
			{
				metalCount++;
				minMetal = std::min<int>(minMetal, m);
				maxMetal = std::max<int>(maxMetal, m);
				M.push_back(z);
				M.push_back(x);
			}
			else
			{
				nonMetalCount++;
			}

			metalmap[z * HMX + x] = m;
			avgMetal += m;
		}
	}

	avgMetal /= (metalCount + nonMetalCount);


	/*
	if (IsMetalMap()) 
	{
		cInt step = std::max(R + R, 4);

		for (Uint32 z = R; z < (Z - R); z += step) {
			for (Uint32 x = R; x < (X - R); x += step) {
				if (metalmap[ID(x, z)] > 1) 
				{
					cUint32 wx = MAP_METAL_2_WORLD(x) << 1;
					cUint32 wz = MAP_METAL_2_WORLD(z) << 1;
					cFloat wy = rcb->GetElevation(wx, wz);
					const float3 metalspot(wx, wy, wz);

					GameMap::metalspots.push_back(metalspot);

					#ifdef GAMEMAP_DEBUG
					rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
					#endif
				}
			}
		}
	}
	else {
	*/
	{
		R++;

		while (true) {
			float maxSaturation = 0.0f;
			bool mexSpotFound = false;

			int bestSpotX = 0;
			int bestSpotZ = 0;

			// Using a greedy approach, find the best metalspot
			for (size_t i = 0; i < M.size(); i += 2) 
			{
				cInt z = M[i    ];
				cInt x = M[i + 1];

				if (metalmap[z * HMX + x] == 0)
					continue;

				float saturation = 0.0f;
				float sum = 0.0f;

				for (size_t c = 0; c < circle.size(); c += 2) 
				{
					cUint32 idx = (z + circle[c]) * HMX + (x + circle[c + 1]);
					rcUint8 m = metalmap[idx];

					saturation += (m * (R - sqrtCircle[c / 2]));
					sum        += m;
				}

				if (saturation > maxSaturation && sum > minimum) 
				{
					bestSpotX = x;
					bestSpotZ = z;
					maxSaturation = saturation;
					mexSpotFound = true;
				}
			}

			// No more mex spots
			if (!mexSpotFound)
				break;

			// "Erase" metal that would be claimed by
			// an extractor placed at <bestSpot{X,Z}>
			for (Uint32 c = 0; c < circle.size(); c += 2) {
				cUint32 idx = (bestSpotZ + circle[c]) * HMX + (bestSpotX + circle[c + 1]);
				metalmap[idx] = 0;
			}


			// Increase to world size
			cUint32 wx = MAP_METAL_2_WORLD(bestSpotX) << 1;
			cUint32 wz = MAP_METAL_2_WORLD(bestSpotZ) << 1;
			cFloat wy = rcb->GetElevation(wx, wz);

			// Store metal spot
			GameMap::metalspots.push_back(float3(wx, wy, wz));

			#ifdef GAMEMAP_DEBUG
			rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
			#endif
		}
	}
}


void GameMap::CalcMapHeightFeatures() {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	// Compute some height features
	cUint32 HMX = rcb->GetMapWidth();
	cUint32 HMZ = rcb->GetMapHeight();
	pcFloat hm = rcb->GetHeightMap();

	float fmin =  std::numeric_limits<float>::max();
	float fmax = -std::numeric_limits<float>::max();
	float fsum = 0.0f;

	Uint32 count = 0;
	Uint32 total = 0;

	// Calculate the sum, min and max
	for (int z = 0; z < HMZ; z++) {
		for (int x = 0; x < HMX; x++) {
			cFloat h = hm[z * HMX + x];

			if (h >= 0.0f) 
			{
				fsum += h;
				fmin = std::min<float>(fmin, h);
				fmax = std::max<float>(fmax, h);
				count++;
			}

			total++;
		}
	}

	cFloat favg = fsum / count;

	// Calculate the variance
	for (int z = 0; z < HMZ; z++) {
		for (int x = 0; x < HMX; x++) {
			cFloat h = hm[z * HMX + x];

			if (h >= 0.0f) 
			{
				heightVariance += ((h / fsum) * std::pow<float>((h - favg), 2.0f));
			}
		}
	}

	// Calculate amount of water in [0, 1]
	waterAmount = 1.0f - (count / float(total));
}
