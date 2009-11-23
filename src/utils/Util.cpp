#include <cassert>
#include <string>
#include <cstring>

#include "ExternalAI/IAICallback.h"
#include "./Util.hpp"

namespace util {
	std::string GetAbsFileName(IAICallback* cb, const std::string& relFileName, bool readonly) {
		char        dst[1024] = {0};
		const char* src       = relFileName.c_str();
		const int   len       = relFileName.size();

		// last char ('\0') in dst
		// should not be overwritten
		assert(len < (1024 - 1));
		memcpy(dst, src, len);

		// get the absolute path to the file
		// (and create folders along the way)
		if (readonly)
			cb->GetValue(AIVAL_LOCATE_FILE_R, dst);
		else
			cb->GetValue(AIVAL_LOCATE_FILE_W, dst);

		return (std::string(dst));
	}

	template<typename T>
	float average(std::list<T> &list) {
		float sum = 0.0f;
		typename std::list<T>::iterator i;
		for (i = list.begin(); i != list.end(); i++)
			sum += *i;

		return sum / list.size();
	}

	unsigned int CountOneBits(unsigned int n) {
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
}
