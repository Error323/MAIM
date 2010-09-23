#include <fstream>
#include <map>

#include "../main/HAIInterface.hpp"

#include "./AILuaHeaders.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaCallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../main/DFolders.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Util.hpp"



LuaModule* LuaModuleLoader::GetModule(const AIUnitDef* def, unsigned int priority) {
	LuaModule* module = ObjectFactory<LuaModule>::Instance();

	/*
	// need to check these against registry (built at load-time)
	// of class-masks advertised by available Lua script modules
	//     def->typeMask
	//     def->terrainMask
	//     def->weaponMask
	//     def->roleMask
	// const std::string& moduleScript = GetModuleScriptName(def, priority);

	module->LoadState(moduleScript, priority);

	MAI_ASSERT(module->IsValid());
	*/
	module->SetPriority(priority);
	return module;
}

lua_State* LuaModuleLoader::LoadLuaModule(const std::string& moduleBaseName) {
	if (luaStates.find(moduleBaseName) != luaStates.end()) {
		return luaStates[moduleBaseName];
	}

	AIHelper* aih = AIHelper::GetActiveInstance();
	IAICallback* rcb = aih->GetCallbackHandler();

	const std::string modFileName = util::StringStripSpaces(rcb->GetModName());

	const std::string defModuleFileNameRel = (AI_LUA_DIR                    ) + ("Def" + moduleBaseName + ".lua");
	const std::string modModuleFileNameRel = (AI_LUA_DIR + modFileName + "/") + ("Mod" + moduleBaseName + ".lua");
	const std::string defModuleFileNameAbs = util::GetAbsFileName(rcb, defModuleFileNameRel, true);
	const std::string modModuleFileNameAbs = util::GetAbsFileName(rcb, modModuleFileNameRel, true);

	std::ifstream defModuleFileStream; defModuleFileStream.open(defModuleFileNameAbs.c_str());
	std::ifstream modModuleFileStream; modModuleFileStream.open(modModuleFileNameAbs.c_str());

	std::string luaScript;

	// either an AI-default or a mod-specific module should exist
	MAI_ASSERT(defModuleFileStream.good() || modModuleFileStream.good());

	if (modModuleFileStream.good()) {
		aih->GetLogger()->Log("[LuaModuleLoader] loading mod-specific module file \"");
		aih->GetLogger()->Log(modModuleFileNameAbs + "\n");
		luaScript = modModuleFileNameAbs;
	} else if (defModuleFileStream.good()) {
		// load the AI-default module only if no mod-specific one is present
		aih->GetLogger()->Log("[LuaModuleLoader] loading AI-default module file \"");
		aih->GetLogger()->Log(defModuleFileNameAbs + "\n");
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
		aih->GetLogger()->Log(std::string(lua_tostring(luaState, -1)) + "\n");
		lua_pop(luaState, 1);
		return NULL;
	} else {
		MAI_ASSERT(lua_gettop(luaState) == 0);

		// register the callbacks for this state
		lua_newtable(luaState); // AI = {}
			lua_pushstring(luaState, "EcoState");
			lua_newtable(luaState); // EcoState = {}
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				lua_pushstring(luaState, "IsStallingMetal");
				lua_pushcfunction(luaState, LuaCallBackHandler::EcoStateIsStallingMetal);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingMetal"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);

				lua_pushstring(luaState, "IsStallingEnergy");
				lua_pushcfunction(luaState, LuaCallBackHandler::EcoStateIsStallingEnergy);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingEnergy"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);
			lua_settable(luaState, -3); // AI["EcoState"] = EcoState
			MAI_ASSERT(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "GameMap");
			lua_newtable(luaState); // GameMap = {}
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				lua_pushstring(luaState, "GetAmountOfLand");
				lua_pushcfunction(luaState, LuaCallBackHandler::GameMapGetAmountOfLand);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // GameMap["GetAmountOfLand"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);

				lua_pushstring(luaState, "GetAmountOfWater");
				lua_pushcfunction(luaState, LuaCallBackHandler::GameMapGetAmountOfWater);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // GameMap["GetAmountOfWater"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);
			lua_settable(luaState, -3); // AI["GameMap"] = GameMap
			MAI_ASSERT(lua_gettop(luaState) == 1);

		// add the AI root table to the global environment
		lua_setglobal(luaState, "AI");
		MAI_ASSERT(lua_gettop(luaState) == 0);

		// lua_register(L, name, func) is short-hand macro for
		// lua_pushcfunction(L, func) + lua_setglobal(L, name)
		// do we want any functions available in the global env (_G)?

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
