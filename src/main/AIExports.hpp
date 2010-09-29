#ifndef AI_EXPORTS_HDR
#define AI_EXPORTS_HDR

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

struct SSkirmishAICallback;

// for a list of the functions that have to be exported,
// see struct SSkirmishAILibrary in "ExternalAI/Interface/SSkirmishAILibrary.h"

EXPORT(int) init(int skirmishAIId, const SSkirmishAICallback* callback);
EXPORT(int) release(int skirmishAIId);
EXPORT(int) handleEvent(int skirmishAIId, int topic, const void* data);

const char* aiexport_getVersion();

#endif
