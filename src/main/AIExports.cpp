#include <map>

// AI interface headers
#include "ExternalAI/Interface/SSkirmishAILibrary.h"
#include "ExternalAI/Interface/SSkirmishAICallback.h"
#include "../Wrappers/LegacyCpp/AIGlobalAI.h"
#include "../Wrappers/CUtils/Util.h"

// MAI headers
#include "./AIExports.hpp"
#include "./AIMain.hpp"

static std::map<int, CAIGlobalAI*> aiInstances;
static std::map<int, const struct SSkirmishAICallback*> teamCallbacks;



EXPORT(int) init(int teamId, const struct SSkirmishAICallback* callback) {
	if (aiInstances.find(teamId) != aiInstances.end()) {
		return -1;
	}

	teamCallbacks[teamId] = callback;

	// CAIGlobalAI is the Legacy C++ wrapper
	aiInstances[teamId] = new CAIGlobalAI(teamId, new AIMain());

	return 0;
}

EXPORT(int) release(int teamId) {
	if (aiInstances.find(teamId) == aiInstances.end()) {
		return -1;
	}

	delete aiInstances[teamId];
	aiInstances[teamId] = NULL;
	aiInstances.erase(teamId);

	return 0;
}

EXPORT(int) handleEvent(int teamId, int topic, const void* data) {
	if (teamId >= 0 && aiInstances.find(teamId) != aiInstances.end()) {
		return aiInstances[teamId]->handleEvent(topic, data);
	}

	// no AI for that team, so return error.
	return -1;
}



///////////////////////////////////////////////////////
// methods from here on are for AI internal use only //
///////////////////////////////////////////////////////
const char* aiexport_getVersion(int teamId) {
	return teamCallbacks[teamId]->Clb_SkirmishAI_Info_getValueByKey(teamId, SKIRMISH_AI_PROPERTY_VERSION);
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
	return teamCallbacks[teamId]->Clb_SkirmishAI_OptionValues_getValueByKey(teamId, key);
}
*/
