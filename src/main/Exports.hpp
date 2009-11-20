#ifndef EXPORTS_HDR
#define EXPORTS_HDR

// check if the correct defines are set by the build system
#if !defined BUILDING_SKIRMISH_AI
#	error BUILDING_SKIRMISH_AI should be defined when building Skirmish AIs
#endif
#if !defined BUILDING_AI
#	error BUILDING_AI should be defined when building Skirmish AIs
#endif
#if defined BUILDING_AI_INTERFACE
#	error BUILDING_AI_INTERFACE should not be defined when building Skirmish AIs
#endif
#if defined SYNCIFY
#	error SYNCIFY should not be defined when building Skirmish AIs
#endif

#include "ExternalAI/Interface/aidefines.h"
#include "ExternalAI/Interface/ELevelOfSupport.h"

struct SSkirmishAICallback;

// for a list of the functions that have to be exported,
// see struct SSkirmishAILibrary in "ExternalAI/Interface/SSkirmishAILibrary.h"

// static AI library methods (optional to implement)
EXPORT(enum LevelOfSupport) getLevelOfSupportFor(int, const char*, int, const char*, const char*);

// team instance functions
EXPORT(int) init(int teamId, const struct SSkirmishAICallback* callback);
EXPORT(int) release(int teamId);
EXPORT(int) handleEvent(int teamId, int topic, const void* data);

// methods from here on are for AI internal use only
const char* aiexport_getVersion(int teamId);
// const char* aiexport_getMyInfo(const char* key);
// const char* aiexport_getDataDir(bool absoluteAndWriteable);
// const char* aiexport_getMyOption(int teamId, const char* key);

#endif
