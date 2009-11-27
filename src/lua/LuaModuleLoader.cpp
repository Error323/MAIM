#include <fstream>
#include <map>
#include <cassert>

#include "ExternalAI/IAICallback.h"

#include "./LuaModuleLoader.hpp"
#include "./LuaAICallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../main/AILua.hpp"
#include "../main/DFolders.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"



// typical call-sequence (from eg. groupHolder)
//   LuaModule* module = ReusableObjectFactory<LuaModule>::Instance();
//   module->LoadState("StaticBuilderModule");
//       calls luaModuleLoader->LoadLuaModule("StaticBuilderModule");
//   if (module->IsValid())
//       group->AddModule(module);
//
lua_State* LuaModuleLoader::LoadLuaModule(const std::string& moduleBaseName) {
	if (luaStates.find(moduleBaseName) != luaStates.end()) {
		return luaStates[moduleBaseName];
	}

	const std::string modFileName = util::StringStripSpaces(aih->rcb->GetModName());

	const std::string defModuleFileNameRel = (AI_LUA_DIR                    ) + ("Def" + moduleBaseName + ".lua");
	const std::string modModuleFileNameRel = (AI_LUA_DIR + modFileName + "/") + ("Mod" + moduleBaseName + ".lua");
	const std::string defModuleFileNameAbs = util::GetAbsFileName(aih->rcb, defModuleFileNameRel, true);
	const std::string modModuleFileNameAbs = util::GetAbsFileName(aih->rcb, modModuleFileNameRel, true);

	std::ifstream defModuleFileStream; defModuleFileStream.open(defModuleFileNameAbs.c_str());
	std::ifstream modModuleFileStream; modModuleFileStream.open(modModuleFileNameAbs.c_str());

	std::string luaScript;

	// either an AI-default or a mod-specific module should exist
	assert(defModuleFileStream.good() || modModuleFileStream.good());

	if (modModuleFileStream.good()) {
		aih->logger->Log("[LuaModuleLoader] loading mod-specific module file \"");
		aih->logger->Log(modModuleFileNameAbs + "\n");
		luaScript = modModuleFileNameAbs;
	} else if (defModuleFileStream.good()) {
		// load the AI-default module only if no mod-specific one is present
		aih->logger->Log("[LuaModuleLoader] loading AI-default module file \"");
		aih->logger->Log(defModuleFileNameAbs + "\n");
		luaScript = defModuleFileNameAbs;
	}

	defModuleFileStream.close();
	modModuleFileStream.close();


	lua_State* luaState = lua_open();
	luaL_openlibs(luaState);

	luaStates[moduleBaseName] = luaState;


	int loadErr = 0;   // 0 | LUA_ERRFILE | LUA_ERRSYNTAX | LUA_ERRMEM
	int callErr = 0;   // 0 | LUA_ERRRUN  | LUA_ERRMEM    | LUA_ERRERR

	if ((loadErr = luaL_loadfile(luaState, luaScript.c_str())) != 0 || (callErr = lua_pcall(luaState, 0, 0, 0)) != 0) {
		aih->logger->Log(std::string(lua_tostring(luaState, -1)) + "\n");
		lua_pop(luaState, 1);
		return NULL;
	} else {
		assert(lua_gettop(luaState) == 0);

		// register the callbacks for this state
		lua_newtable(luaState); // AI = {}
			lua_pushstring(luaState, "EcoState");
			lua_newtable(luaState); // EcoState = {}
			assert(lua_istable(luaState, -3));
			assert(lua_istable(luaState, -1));
				lua_pushstring(luaState, "IsStallingMetal");
				lua_pushcfunction(luaState, LuaAICallBackHandler::EcoStateIsStallingMetal);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingMetal"] = func
				assert(lua_gettop(luaState) == 3);

				lua_pushstring(luaState, "IsStallingEnergy");
				lua_pushcfunction(luaState, LuaAICallBackHandler::EcoStateIsStallingEnergy);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingEnergy"] = func
				assert(lua_gettop(luaState) == 3);
			lua_settable(luaState, -3); // AI["EcoState"] = EcoState
			assert(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "GameMap");
			lua_newtable(luaState); // GameMap = {}
			assert(lua_istable(luaState, -3));
			assert(lua_istable(luaState, -1));
				lua_pushstring(luaState, "GetAmountOfLand");
				lua_pushcfunction(luaState, LuaAICallBackHandler::GameMapGetAmountOfLand);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // GameMap["GetAmountOfLand"] = func
				assert(lua_gettop(luaState) == 3);

				lua_pushstring(luaState, "GetAmountOfWater");
				lua_pushcfunction(luaState, LuaAICallBackHandler::GameMapGetAmountOfWater);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // GameMap["GetAmountOfWater"] = func
				assert(lua_gettop(luaState) == 3);
			lua_settable(luaState, -3); // AI["GameMap"] = GameMap
			assert(lua_gettop(luaState) == 1);

		// add the AI root table to the global environment
		lua_setglobal(luaState, "AI");
		assert(lua_gettop(luaState) == 0);

		// lua_register(L, name, func) is short-hand macro for
		// lua_pushcfunction(L, func) + lua_setglobal(L, name)
		// do we want any functions available in the global env?

		return luaState;
	}
}

LuaModuleLoader::~LuaModuleLoader() {
	// close all cached Lua states
	for (std::map<std::string, lua_State*>::iterator it = luaStates.begin(); it != luaStates.end(); it++) {
		lua_close(it->second);
	}

	luaStates.clear();
}
