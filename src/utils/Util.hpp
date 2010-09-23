#ifndef UTIL
#define UTIL

#include <list>
#include <set>
#include <string>

#include "../main/Types.hpp"

DECLARE_CLASS(IAICallback)

namespace util {
	std::string GetAbsFileName(pIAICallback, rcString, cBool readonly = false);
	int GetFilesInDir(rcString dir, rvString files);

	void StringToLowerInPlace(rString);
	std::string StringToLower(String);
	std::string StringStripSpaces(rcString);

	float WeightedAverage(std::list<float>&, std::list<float>&);

	float GaussDens(float, float mu = 0.0f, float sigma = 1.0f);

	unsigned int CountOneBits(cUint32 n);

	/** 
	 * Determines if A is a binary subset of B 
	 *
	 * @param unsigned, binary mask A
	 * @param unsigned, binary mask B
	 * @return bool, A \subseteq B
	 */
	bool IsBinarySubset(cUint32 A, cUint32 B);

	template<typename T> std::set<T> IntersectSets(const std::set<T>& s1, const std::set<T>& s2) {
		typename std::set<T> r;
		typename std::set<T>::const_iterator sit;

		for (sit = s1.begin(); sit != s1.end(); sit++) {
			if (s2.find(*sit) != s2.end()) {
				r.insert(*sit);
			}
		}

		return r;
	}

	template<typename T> std::set<T> UnionSets(const std::set<T>& s1, const std::set<T>& s2) {
		typename std::set<T> r;
		typename std::set<T>::const_iterator sit;

		for (sit = s1.begin(); sit != s1.end(); sit++) { r.insert(*sit); }
		for (sit = s2.begin(); sit != s2.end(); sit++) { r.insert(*sit); }

		return r;
	}
}

#endif
