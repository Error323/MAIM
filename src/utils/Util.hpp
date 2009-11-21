#ifndef UTIL
#define UTIL

#include <set>

class IAICallback;
std::string GetAbsFileName(IAICallback*, const std::string&, bool readonly = false);

static unsigned int CountOneBits(unsigned int n) {
	const int S[] = {1, 2, 4, 8, 16};
	const int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};
	int c = n;
	c = ((c >> S[0]) & B[0]) + (c & B[0]);
	c = ((c >> S[1]) & B[1]) + (c & B[1]);
	c = ((c >> S[2]) & B[2]) + (c & B[2]);
	c = ((c >> S[3]) & B[3]) + (c & B[3]);
	c = ((c >> S[4]) & B[4]) + (c & B[4]);
	return c;
}

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

#endif
