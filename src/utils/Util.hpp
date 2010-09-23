#ifndef UTIL
#define UTIL

#include <list>
#include <set>
#include <string>

#include "../main/Types.hpp"

DECLARE_CLASS(IAICallback)

#define EMPTY_BINARY_INTERSECTION(A, B) (((A) & (B)) == 0)
#define BINARY_SUBSET(A, B)             (((A) & (B)) == (A))

namespace util {
	std::string GetAbsFileName(pIAICallback, rcString, cBool readonly = false);
	int GetFilesInDir(rcString dir, rvString files);

	void StringToLowerInPlace(rString);
	std::string StringToLower(String);
	std::string StringStripSpaces(rcString);

	float WeightedAverage(std::list<float>&, std::list<float>&);

	float GaussDens(float, float mu = 0.0f, float sigma = 1.0f);

	/**@brief fill with unit-type masks to see wether a AIUnitDef meets the filters */
	bool AreSuitedSubjects(rcvUint32 subjects, rcvUint32 includes, rcvUint32 excludes);

	/**@brief fill with unit-type mask to see wether a AIUnitDef meets the filters */
	bool IsSuitedSubject(cUint32 subject, cUint32 include, cUint32 exclude);

	/**@brief counts bits set to 1 */
	Uint32 CountOneBits(cUint32 n);

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
