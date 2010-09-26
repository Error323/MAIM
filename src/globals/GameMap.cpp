#include <limits>
#include <iostream>
#include <map>

#include "System/float3.h"
#include "LegacyCpp/IAICallback.h"
#include "LegacyCpp/UnitDef.h"

#include "./GameMap.hpp"
#include "../main/AIHelper.hpp"
#include "../groups/AIGroup.hpp"
#include "../utils/Logger.hpp"

#define ID(x, z) ((z) * X + (x))
#define METAL_THRESHOLD 32

std::list<float3> GameMap::geospots;
std::list<float3> GameMap::metalfeatures;
std::list<float3> GameMap::energyfeatures;
std::list<float3> GameMap::metalspots;

void GameMap::Init() {
	heightVariance = 0.0f;
	waterAmount    = 0.0f;
	metalAmount    = 0.0f;

	CalcMapHeightFeatures();
	CalcMetalSpots();

	LOG_BASIC("[GameMap::HasMetalSpots] " << HasMetalSpots() << "\n");
	LOG_BASIC("[GameMap::HasGeoSpots] " << HasGeoSpots() << "\n");
	LOG_BASIC("[GameMap::HasEnergyFeatures] " << HasEnergyFeatures() << "\n");
	LOG_BASIC("[GameMap::HasMetalFeatures] " << HasMetalFeatures() << "\n");
	LOG_BASIC("[GameMap::IsKbotMap] " << IsKbotMap() << "\n");
	LOG_BASIC("[GameMap::IsVehicleMap] " << IsVehicleMap() << "\n");
	LOG_BASIC("[GameMap::IsMetalMap] " << IsMetalMap() << "\n");
	LOG_BASIC("[GameMap::HeightVariance] " << GetHeightVariance() << "\n");
	LOG_BASIC("[GameMap::GetAmountOfWater] " << GetAmountOfWater() << "\n");
	LOG_BASIC("[GameMap::GetAmountOfLand] " << GetAmountOfLand() << "\n");
	LOG_BASIC("[GameMap::CalcMetalSpots] found " << metalspots.size() << " metalspots\n");
}

float3 GameMap::GetClosestOpenMetalSpot(pAIGroup group) {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	const float3 gpos = group->GetPos();

	std::map<float, float3> candidates;
	std::list<float3>::iterator i;

	for (i = metalspots.begin(); i != metalspots.end(); i++) 
	{
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
			float radius = (gpos - *i).Length2D();
			candidates[radius] = *i;
		}
	}
	return candidates.empty() ? ZeroVector : candidates.begin()->second;
}


void GameMap::CalcMetalSpots() {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	cInt METAL2REAL = 32.0f;
	cInt X = int(rcb->GetMapWidth() >> 2);
	cInt Z = int(rcb->GetMapHeight() >> 2);
	 Int R = int(round(rcb->GetExtractorRadius() / 32.0f));

	pcUint8 metalmapData = rcb->GetMetalMap();
	vUint8 metalmap(X * Z, 0);
		
	// Calculate circular stamp
	std::vector<int> circle;
	std::vector<float> sqrtCircle;

	for (int i = -R; i <= R; i++) 
	{
		for (int j = -R; j <= R; j++) 
		{
			cFloat r = sqrt((float)i*i + j*j);
			if (r > R) continue;
			circle.push_back(i);
			circle.push_back(j);
			sqrtCircle.push_back(r);
		}
	}

	cFloat minimum = 10.0f * M_PI * R * R;

	// Copy metalmap to mutable metalmap
	vInt M;
	avgMetal = 0;
	minMetal = std::numeric_limits<int>::max();
	maxMetal = std::numeric_limits<int>::min();

	for (int z = R; z < Z-R; z++) 
	{
		for (int x = R; x < X-R; x++) 
		{
			Uint8 m = 0;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (metalmapData[(z*2+i)*X*2+(x*2+j)] > 1)
					{
						m = std::max<int>(metalmapData[(z*2+i)*X*2+(x*2+j)], m);
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
			metalmap[ID(x, z)] = m;
			avgMetal += m;
		}
	}
	avgMetal /= (metalCount + nonMetalCount);


	if (IsMetalMap()) 
	{
		int step = (R+R) > 4 ? (R+R) : 4;
		for (int z = R; z < Z-R; z+=step) 
		{
			for (int x = R; x < X-R; x+=step) 
			{
				if (metalmap[ID(x, z)] > 1) 
				{
					float3 metalspot(x * METAL2REAL, rcb->GetElevation(x * METAL2REAL, z * METAL2REAL), z * METAL2REAL);
					GameMap::metalspots.push_back(metalspot);

					#ifdef GAMEMAP_DEBUG
					rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
					#endif
				}
			}
		}
	}
	else {
		R++;

		while (true) 
		{
			float highestSaturation = 0.0f, saturation, sum;
			int bestX = 0, bestZ = 0;
			bool mexSpotFound = false;

			// Using a greedy approach, find the best metalspot
			for (size_t i = 0; i < M.size(); i+=2) 
			{
				int z = M[i]; int x = M[i+1];
				if (metalmap[ID(x, z)] == 0)
				{
					continue;
				}

				saturation = 0.0f; sum = 0.0f;
				for (size_t c = 0; c < circle.size(); c+=2) 
				{
					rcUint8 m = metalmap[ID(x + circle[c + 1], z + circle[c])];
					saturation += m * (R-sqrtCircle[c/2]);
					sum        += m;
				}

				if (saturation > highestSaturation && sum > minimum) 
				{
					bestX = x; bestZ = z;
					highestSaturation = saturation;
					mexSpotFound = true;
				}
			}

			// No more mex spots
			if (!mexSpotFound) break;

			// "Erase" metal under the bestX bestZ radius
			for (Uint32 c = 0; c < circle.size(); c+=2)
			{
				metalmap[ID(circle[c + 1] + bestX, circle[c] + bestZ)] = 0;
			}
			
			// Increase to world size
			bestX *= METAL2REAL; bestZ *= METAL2REAL;

			// Store metal spot
			float3 metalspot(bestX, rcb->GetElevation(bestX,bestZ), bestZ);
			GameMap::metalspots.push_back(metalspot);

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
	cInt X = rcb->GetMapWidth();
	cInt Z = rcb->GetMapHeight();
	pcFloat hm = rcb->GetHeightMap();

	float fmin =  std::numeric_limits<float>::max();
	float fmax = -std::numeric_limits<float>::max();
	float fsum = 0.0f;

	Uint32 count = 0;
	Uint32 total = 0;

	// Calculate the sum, min and max
	for (int z = 0; z < Z; z++) 
	{
		for (int x = 0; x < X; x++) 
		{
			float h = hm[ID(x, z)];
			if (h >= 0.0f) 
			{
				fsum += h;
				fmin = std::min<float>(fmin,h);
				fmax = std::max<float>(fmax,h);
				count++;
			}
			total++;
		}
	}

	float favg = fsum / count;

	// Calculate the variance
	for (int z = 0; z < Z; z++) 
	{
		for (int x = 0; x < X; x++) 
		{
			float h = hm[ID(x, z)];
			if (h >= 0.0f) 
			{
				heightVariance += (h / fsum) * std::pow<float>((h - favg), 2.0f);
			}
		}
	}

	// Calculate amount of water in [0,1]
	waterAmount = 1.0f - (count / float(total));
}
