#include "./GameMap.hpp"

#include <limits>
#include <iostream>
#include <map>

#include "../main/HEngine.hpp"
#include "../main/HAIInterface.hpp"
#include "../main/AIHelper.hpp"
#include "../main/DMacros.hpp"
#include "../groups/AIGroup.hpp"

#define METAL_THRESHOLD 32

std::list<float3> GameMap::geospots;
std::list<float3> GameMap::metalfeatures;
std::list<float3> GameMap::energyfeatures;
std::list<float3> GameMap::metalspots;

void GameMap::Init(pAIHelper aih) {
	this->aih = aih;

	heightVariance = 0.0f;
	waterAmount    = 0.0f;
	metalAmount    = 0.0f;

	CalcMapHeightFeatures();
	CalcMetalSpots();
}

float3 GameMap::GetClosestOpenMetalSpot(pAIGroup group) {
	std::map<float, float3> candidates;
	float3 gpos = group->GetPos();
	std::list<float3>::iterator i;
	for (i = metalspots.begin(); i != metalspots.end(); i++) 
	{
		int units[50];
		int numUnits = aih->rcb->GetFriendlyUnits(units, *i, aih->rcb->GetExtractorRadius(), 50);
		bool taken = false;
		for (int j = 0; j < numUnits; j++) 
		{
			if (aih->rcb->GetUnitDef(units[j])->extractsMetal > 0.0f) 
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
	cInt METAL2REAL = 32.0f;
	int X = int(aih->rcb->GetMapWidth()/4);
	int Z = int(aih->rcb->GetMapHeight()/4);
	int R = int(round(aih->rcb->GetExtractorRadius() / 32.0f));
	pcUint8 metalmapData = aih->rcb->GetMetalMap();
	Uint8 metalmap[X*Z];
		
	// Calculate circular stamp
	std::vector<int> circle;
	std::vector<float> sqrtCircle;
	for (int i = -R; i <= R; i++) 
	{
		for (int j = -R; j <= R; j++) 
		{
			float r = sqrt((float)i*i + j*j);
			if (r > R) continue;
			circle.push_back(i);
			circle.push_back(j);
			sqrtCircle.push_back(r);
		}
	}
	cFloat minimum = 10*M_PI*R*R;

	// Copy metalmap to mutable metalmap
	std::vector<int> M;
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
			metalmap[ID(x,z)] = m;
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
				if (metalmap[ID(x,z)] > 1) 
				{
					float3 metalspot(x*METAL2REAL, aih->rcb->GetElevation(x*METAL2REAL,z*METAL2REAL), z*METAL2REAL);
					GameMap::metalspots.push_back(metalspot);
					if (true)
					{
						aih->rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
					}
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
				if (metalmap[ID(x,z)] == 0)
				{
					continue;
				}

				saturation = 0.0f; sum = 0.0f;
				for (size_t c = 0; c < circle.size(); c+=2) 
				{
					rcUint8 m = metalmap[ID(x+circle[c+1],z+circle[c])];
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
				metalmap[ID(circle[c+1]+bestX,circle[c]+bestZ)] = 0;
			}
			
			// Increase to world size
			bestX *= METAL2REAL; bestZ *= METAL2REAL;

			// Store metal spot
			float3 metalspot(bestX, aih->rcb->GetElevation(bestX,bestZ), bestZ);
			GameMap::metalspots.push_back(metalspot);

			if (true)
			{
				aih->rcb->DrawUnit("armmex", metalspot, 0.0f, 10000, 0, false, false, 0);
			}
		}
	}

	std::string maptype;
	if(IsMetalMap())
	{
		maptype = "speedmetal";
	}
	else if (nonMetalCount == 0)
	{
		maptype = "non-metalmap";
	}
	else
	{
		maptype = "normal metalmap";
	}
}


void GameMap::CalcMapHeightFeatures() {
	// Compute some height features
	cInt X = int(aih->rcb->GetMapWidth());
	cInt Z = int(aih->rcb->GetMapHeight());
	pcFloat hm = aih->rcb->GetHeightMap();

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
			float h = hm[ID(x,z)];
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
			float h = hm[ID(x,z)];
			if (h >= 0.0f) 
			{
				heightVariance += (h/fsum) * std::pow<float>((h - favg), 2.0f);
			}
		}
	}

	// Calculate amount of water in [0,1]
	waterAmount = 1.0f - (count / float(total));
}
