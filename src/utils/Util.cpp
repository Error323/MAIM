#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "../main/HAIInterface.hpp"
#include "./Util.hpp"

namespace util {
	String GetAbsFileName(pIAICallback cb, rcString relFileName, cBool readonly) {
		char   dst[1024] = {0};
		pcChar src       = relFileName.c_str();
		cInt   len       = relFileName.size();

		// last char ('\0') in dst
		// should not be overwritten
		assert(len < (1024 - 1));
		memcpy(dst, src, len);

		// get the absolute path to the file
		// (and create folders along the way)
		if (readonly) {
			cb->GetValue(AIVAL_LOCATE_FILE_R, dst);
		} else {
			cb->GetValue(AIVAL_LOCATE_FILE_W, dst);
		}

		return (String(dst));
	}


	void StringToLowerInPlace(rString s) {
		std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))tolower);
	}
	String StringToLower(String s) {
		StringToLowerInPlace(s);
		return s;
	}

	String StringStripSpaces(rcString s1) {
		String s2(s1);
		s2.erase(remove_if(s2.begin(), s2.end(), isspace), s2.end());
		return s2;
	}


	float WeightedAverage(std::list<float>& V, std::list<float>& W) {
		float wavg = 0.0f;
		std::list<float>::const_iterator v, w;
		for (w = W.begin(), v = V.begin(); v != V.end() && w != W.end(); w++, v++)
			wavg += ((*w) * (*v));

		return wavg;
	}


	bool IsBinarySubset(cUint32 A, cUint32 B) {
		cUint32 Acount = CountOneBits(A);
		cUint32 AandBcount  = CountOneBits(A&B);

		return (Acount == AandBcount);
	}

	unsigned int CountOneBits(cUint32 n) {
		cInt S[] = {1, 2, 4, 8, 16};
		cInt B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};
		int c = n;
		c = ((c >> S[0]) & B[0]) + (c & B[0]);
		c = ((c >> S[1]) & B[1]) + (c & B[1]);
		c = ((c >> S[2]) & B[2]) + (c & B[2]);
		c = ((c >> S[3]) & B[3]) + (c & B[3]);
		c = ((c >> S[4]) & B[4]) + (c & B[4]);
		return c;
	}

	float GaussDens(cFloat x, cFloat mu, cFloat sigma) {
		cFloat a = 1.0f / (sigma * std::sqrt(2.0f * M_PI));
		cFloat b = std::exp(-(((x - mu) * (x - mu)) / (2.0f * sigma * sigma)));
		return (a * b);
	}
}
