#include <fstream>
#include <map>
#include <cassert>

#include "ExternalAI/IAICallback.h"

#include "./LuaModuleLoader.hpp"
#include "../modules/LuaModule.hpp"
#include "../main/AIHelper.hpp"
#include "../main/AILua.hpp"
#include "../main/DFolders.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"


// eg. luaModuleLoader->LoadLuaModule("StaticBuilderModule");
//
AModule* LuaModuleLoader::LoadLuaModule(const std::string& moduleName) {
	if (luaModules.find(moduleName) != luaModules.end()) {
		return luaModules[moduleName];
	}

	const std::string modName = util::StringStripSpaces(aih->rcb->GetModName());

	const std::string defModuleNameRel = (AI_LUA_DIR                ) + ("Def" + moduleName + ".lua");
	const std::string modModuleNameRel = (AI_LUA_DIR + modName + "/") + ("Mod" + moduleName + ".lua");
	const std::string defModuleNameAbs = util::GetAbsFileName(aih->rcb, defModuleNameRel, true);
	const std::string modModuleNameAbs = util::GetAbsFileName(aih->rcb, modModuleNameRel, true);

	std::ifstream defModuleStream; defModuleStream.open(defModuleNameAbs.c_str());
	std::ifstream modModuleStream; modModuleStream.open(modModuleNameAbs.c_str());

	LuaModule* m = NULL;
	bool loaded = true;

	if (modModuleStream.good()) {
		// m = ReusableObjectFactory<LuaModule>::Instance();
		m = new LuaModule();
		m->Load(modModuleNameAbs);

		aih->logger->Log("[LuaModuleLoader] loaded mod-specific module file \"");
		aih->logger->Log(modModuleNameAbs + "\n");
	} else if (defModuleStream.good()) {
		// load the AI-default module only if no mod-specific one is present
		// m = ReusableObjectFactory<LuaModule>::Instance();
		m = new LuaModule();
		m->Load(defModuleNameAbs);

		aih->logger->Log("[LuaModuleLoader] loaded AI-default module file \"");
		aih->logger->Log(defModuleNameAbs + "\n");
	}

	defModuleStream.close();
	modModuleStream.close();

	// either an AI-default or a mod-specific module should exist
	assert(m != NULL);

	LOG_BASIC(aih->logger, "\tIsValid():     " << (m->IsValid()) << "\n");
	LOG_BASIC(aih->logger, "\tHaveGetName(): " << (m->HaveGetName()) << "\n");
	LOG_BASIC(aih->logger, "\tHaveCanRun():  " << (m->HaveCanRun()) << "\n");
	LOG_BASIC(aih->logger, "\tHaveUpdate():  " << (m->HaveUpdate()) << "\n");


	if (loaded && !m->IsValid()) {
		loaded = false;
	}
	if (loaded && (!m->HaveGetName() || m->GetName() == "")) {
		loaded = false;
	}

	if (loaded) {
		luaModules[moduleName] = m;
		return m;
	} else {
		aih->logger->Log("\tmodule file is invalid, unloading...");
		delete m;
		return NULL;
	}
}

LuaModuleLoader::~LuaModuleLoader() {
	for (std::map<std::string, AModule*>::iterator it = luaModules.begin(); it != luaModules.end(); it++) {
		delete it->second;
	}

	luaModules.clear();
}
