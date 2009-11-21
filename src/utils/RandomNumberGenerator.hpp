#ifndef RANDOM_NUMBER_GENERATOR
#define RANDOM_NUMBER_GENERATOR

#include <cstdlib>
#include <ctime>
#define FRAND_MAX float(RAND_MAX)

struct RandomNumberGenerator {
	RandomNumberGenerator() {
		srand(time(0));
	}

	// random float in range [0, range]
	// (rand() can return RAND_MAX)
	float RandFloat(float range = 1.0f) const {
		range = (range < 0.0f)? -range: range;
		return ((rand() / FRAND_MAX) * range);
	}
	// random int in range [0, (-)range]
	// (rand() can return RAND_MAX)
	int RandInt(int range = 1) const {
		return int((rand() / FRAND_MAX) * range);
	}
};

// Handle
const static RandomNumberGenerator rng;

#endif
