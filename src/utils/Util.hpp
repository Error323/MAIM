#ifndef UTIL
#define UTIL

#include <list>

class IAICallback;

namespace util {
	std::string GetAbsFileName(IAICallback*, const std::string&, bool readonly = false);

	float average(std::list<float> &list);

	unsigned int CountOneBits(unsigned int n);

	/** 
	 * Determines if A is a binary subset of B 
	 *
	 * @param unsigned, binary mask A
	 * @param unsigned, binary mask B
	 * @return bool, A \subseteq B
	 */
	bool IsBinarySubset(unsigned A, unsigned B);
}

#endif
