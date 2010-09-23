#include <cstring>
#include <cmath>
#include <algorithm>

#include <sys/types.h>
#include <dirent.h>

#include "../main/HAIInterface.hpp"
#include "./Util.hpp"
#include "./Debugger.hpp"

namespace util {
	String GetAbsFileName(pIAICallback cb, rcString relFileName, cBool readonly) {
		char   dst[1024] = {0};
		pcChar src       = relFileName.c_str();
		cInt   len       = relFileName.size();

		// last char ('\0') in dst
		// should not be overwritten
		MAI_ASSERT(len < (1024 - 1));
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

	int GetFilesInDir(rcString dir, rvString files) {
		typedef struct dirent DIRENT;

		DIR* dirStream;
		DIRENT* dirEntry;

		if ((dirStream = opendir(dir.c_str())) == NULL) {
			return -1;
		}

		while ((dirEntry = readdir(dirStream)) != NULL) {
			if (dirEntry->d_type == DT_REG) {
				// regular file
				files.push_back(String(dirEntry->d_name));
			}
		}

		closedir(dirStream);
		return 0;
	}


	inline void StringToLowerInPlace(rString s) {
		std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))tolower);
	}
	inline String StringToLower(String s) {
		StringToLowerInPlace(s);
		return s;
	}

	inline String StringStripSpaces(rcString s1) {
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

	bool AreSuitedSubjects(rcvUint32 subjects, rcvUint32 includes, rcvUint32 excludes) {
		cUint32 n = subjects.size();

		MAI_ASSERT(n == includes.size());
		MAI_ASSERT(n == excludes.size());

		for (Uint32 i = 0; i < n; i++)
		{
			if (!IsSuitedSubject(subjects[i], includes[i], excludes[i]))
			{
				return false;
			}
		}

		return true;
	}

	inline bool IsSuitedSubject(cUint32 subject, cUint32 include, cUint32 exclude) {
		return (IsBinarySubset(include, subject) && IsBinaryIntersectionEmpty(subject, exclude));
	}

	inline bool IsBinaryIntersectionEmpty(cUint32 A, cUint32 B) {
		return (A&B) == 0;
	}

	inline bool IsBinarySubset(cUint32 A, cUint32 B) {
		cUint32 Acount      = CountOneBits(A);
		cUint32 AandBcount  = CountOneBits(A&B);

		return (Acount == AandBcount);
	}

	inline unsigned int CountOneBits(cUint32 n) {
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

	inline float GaussDens(cFloat x, cFloat mu, cFloat sigma) {
		cFloat a = 1.0f / (sigma * std::sqrt(2.0f * M_PI));
		cFloat b = std::exp(-(((x - mu) * (x - mu)) / (2.0f * sigma * sigma)));
		return (a * b);
	}
}
