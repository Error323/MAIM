#include "./GameMap.hpp"

#include <float.h>
#include <iostream>
#include <map>

#include "../main/AIHelper.hpp"
#include "../utils/Util.hpp"
#include "../groups/Group.hpp"

#include "Sim/Units/UnitDef.h"
#include "ExternalAI/IAICallback.h"

void GameMap::Init(AIHelper *aih) {
	this->aih = aih;

	heightVariance = 0.0f;
	waterAmount    = 0.0f;

	CalcMapHeightFeatures();
	CalcMetalSpots();
}

float3 GameMap::GetClosestOpenMetalSpot(Group* group) {
	std::map<float, float3> candidates;
	float3 gpos = group->GetPos();
	std::list<float3>::iterator i;
	for (i = metalspots.begin(); i != metalspots.end(); i++) {
		int units[50];
		int numUnits = aih->rcb->GetFriendlyUnits(units, *i, aih->rcb->GetExtractorRadius(), 50);
		bool taken = false;
		for (int j = 0; j < numUnits; j++) {
			if (aih->rcb->GetUnitDef(units[j])->extractsMetal > 0.0f) {
				taken = true;
				break;
			}
		}
		if (!taken) {
			float radius = (gpos - *i).Length2D();
			candidates[radius] = *i;
		}
	}
	return candidates.empty() ? ZeroVector : candidates.begin()->second;
}


void GameMap::CalcMetalSpots() {
	int X = int(aih->rcb->GetMapWidth()/4);
	int Z = int(aih->rcb->GetMapHeight()/4);
	int R = int(round(aih->rcb->GetExtractorRadius() / 32.0f));

	const unsigned char *mm = aih->rcb->GetMetalMap();
	unsigned char metalmap[X*Z];

	// Copy metalmap to mutable metalmap
	for (int z = 0; z < Z; z++)
		for (int x = 0; x < X; x++)
			metalmap[z*X+x] = mm[(z*2)*(X*2)+(x*2)];

	while (true) {
		float highestSaturation = 0.0f;
		int bestX, bestZ;
		bool mexSpotFound = false;

		// Using a greedy approach, find the best metalspot
		for (int z = 0; z < Z; z++) {
			for (int x = 0; x < X; x++) {
				if (metalmap[z*X+x] > 64) {
					mexSpotFound = true;
					float saturation = 0.0f;
					for (int i = -R; i <= R; i++) {
						for (int j = -R; j <= R; j++) {
							int zz = i+z;
							int xx = j+x;
							if (xx < 0 || xx > X-1 || zz < 0 || zz > Z-1)
								continue;
							float r = sqrt(i*i + j*j);
							if (r > R)
								continue;
							saturation += (metalmap[zz*X+xx] * (1.0f / (r+1.0f)));
						}
					}
					if (saturation > highestSaturation) {
						bestX = x;
						bestZ = z;
						highestSaturation = saturation;
					}
				}
			}
		}

		// No more mex spots
		if (!mexSpotFound) break;

		// "Erase" metal under the bestX bestZ radius
		for (int i = -R; i <= R; i++) {
			for (int j = -R; j <= R; j++) {
				int z = i+bestZ;
				int x = j+bestX;
				if (x < 0 || x > X-1 || z < 0 || z > Z-1)
					continue;
				if (sqrt(i*i + j*j) > R)
					continue;
				metalmap[z*X+x] = 0;
			}
		}
		
		// Increase to world size
		bestX *= 32.0f; bestZ *= 32.0f;

		// Store metal spot
		float3 metalspot(bestX, aih->rcb->GetElevation(bestX,bestZ), bestZ);
		metalspots.push_back(metalspot);

		// Debug
		// aih->rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
	}
}


void GameMap::CalcMapHeightFeatures() {
	// Compute some height features
	int X = int(aih->rcb->GetMapWidth());
	int Z = int(aih->rcb->GetMapHeight());
	const float *hm = aih->rcb->GetHeightMap();

	float fmin =  FLT_MAX;
	float fmax = -FLT_MAX;
	float fsum = 0.0f;

	unsigned count = 0;
	unsigned total = 0;
	// Calculate the sum, min and max
	for (int z = 0; z < Z; z++) {
		for (int x = 0; x < X; x++) {
			float h = hm[z*X+x];
			if (h >= 0.0f) {
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
	for (int z = 0; z < Z; z++) {
		for (int x = 0; x < X; x++) {
			float h = hm[z*X+x];
			if (h >= 0.0f) heightVariance += (h/fsum) * std::pow<float>((h - favg), 2.0f);
		}
	}

	// Calculate amount of water in [0,1]
	waterAmount = 1.0f - (count / float(total));
}
