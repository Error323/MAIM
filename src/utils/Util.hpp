#ifndef UTIL
#define UTIL

#include <set>
#include <list>

class IAICallback;

namespace util {
	std::string GetAbsFileName(IAICallback*, const std::string&, bool readonly = false);

	template<typename T>
	float average(std::list<T> &list);

	unsigned int CountOneBits(unsigned int n);
}

#endif
