#ifndef UTIL
#define UTIL

#include <list>

class IAICallback;

namespace util {
	std::string GetAbsFileName(IAICallback*, const std::string&, bool readonly = false);

	float average(std::list<float> &list);

	unsigned int CountOneBits(unsigned int n);
}

#endif
