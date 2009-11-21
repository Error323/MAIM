#include <cassert>
#include <string>
#include <cstring>

#include "ExternalAI/IAICallback.h"
#include "./Util.hpp"

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
