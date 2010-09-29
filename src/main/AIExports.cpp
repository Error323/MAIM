#include <map>

// AI interface headers
#include "ExternalAI/Interface/SSkirmishAILibrary.h"
#include "ExternalAI/Interface/SSkirmishAICallback.h"
#include "../Wrappers/LegacyCpp/AIAI.h"

// MAI headers
#include "./AIExports.hpp"
#include "./AIMain.hpp"

static std::map<int, CAIAI*> aiInstances;
static std::map<int, const SSkirmishAICallback*> aiCallbacks;



EXPORT(int) init(int skirmishAIId, const SSkirmishAICallback* callback) {
	if (aiInstances.find(skirmishAIId) != aiInstances.end()) {
		return -1;
	}

	aiCallbacks[skirmishAIId] = callback;

	// CAIAI is the Legacy C++ wrapper
	aiInstances[skirmishAIId] = new CAIAI(new AIMain());
	return 0;
}

EXPORT(int) release(int skirmishAIId) {
	if (aiInstances.find(skirmishAIId) == aiInstances.end()) {
		return -1;
	}

	delete aiInstances[skirmishAIId];
	aiInstances[skirmishAIId] = NULL;
	aiInstances.erase(skirmishAIId);

	return 0;
}

EXPORT(int) handleEvent(int skirmishAIId, int topic, const void* data) {
	if (aiInstances.find(skirmishAIId) != aiInstances.end()) {
		return aiInstances[skirmishAIId]->handleEvent(topic, data);
	}

	// no AI for that team, so return error.
	return -1;
}



///////////////////////////////////////////////////////
// methods from here on are for AI internal use only //
///////////////////////////////////////////////////////
const char* aiexport_getVersion() {
	const int skirmishAIId = aiCallbacks.begin()->first;
	const SSkirmishAICallback* cb = aiCallbacks[skirmishAIId];
	const char* version = cb->SkirmishAI_Info_getValueByKey(skirmishAIId, SKIRMISH_AI_PROPERTY_VERSION);

	return version;
}

/*
const char* aiexport_getDataDir(bool absoluteAndWriteable) {
	static char* dd_ws_rel   = NULL;
	static char* dd_ws_abs_w = NULL;

	if (absoluteAndWriteable) {
		if (dd_ws_abs_w == NULL) {
			// this is the writeable one, absolute
			dd_ws_abs_w = util_allocStrCpy(firstCallback->Clb_DataDirs_getWriteableDir(firstTeamId));
		}
		return dd_ws_abs_w;
	} else {
		if (dd_ws_rel == NULL) {
			dd_ws_rel = util_allocStrCpy(firstCallback->Clb_DataDirs_getConfigDir(firstTeamId));
			// remove the X, so we end up with a slash at the end
			if (dd_ws_rel != NULL) {
				dd_ws_rel[strlen(dd_ws_rel) -1] = '\0';
			}
		}
		return dd_ws_rel;
	}

	return NULL;
}

const char* aiexport_getMyOption(int teamId, const char* key) {
	return aiCallbacks[teamId]->Clb_SkirmishAI_OptionValues_getValueByKey(teamId, key);
}
*/
