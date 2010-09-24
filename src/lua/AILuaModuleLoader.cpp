#include <string>
#include <fstream>

#include "../main/HAIInterface.hpp"

#include "./AILuaHeaders.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaCallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../main/DFolders.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Util.hpp"



LuaModule* LuaModuleLoader::GetModule(const AIUnitDef* def, unsigned int priority) {
	lua_State* moduleState = NULL;
	LuaModule* module = NULL;
	LuaModule::LuaModuleClass moduleClass;
		moduleClass.typeMask = def->typeMask;
		moduleClass.terrMask = def->terrainMask;
		moduleClass.weapMask = def->weaponMask;
		moduleClass.roleMask = def->roleMask;

	if (luaModules.find(moduleClass) != luaModules.end()) {
		// module = luaModules[moduleClass][priority];
		// moduleState = luaModuleStates[moduleClass][priority];
	} else {
		module = ObjectFactory<LuaModule>::Instance();

		typedef std::map<LuaModule::LuaModuleClass, std::vector<lua_State*> > LuaStateMap;
		typedef std::map<LuaModule::LuaModuleClass, std::vector<lua_State*> >::iterator LuaStateMapIt;

		for (LuaStateMapIt it = luaModuleStates.begin(); it != luaModuleStates.end(); ++it) {
			const LuaModule::LuaModuleClass& lmc = it->first;
			const std::vector<lua_State*>& lmsv = it->second;

			// check if <def>'s module-class is suited for this module
			if (!IS_BINARY_SUBSET(moduleClass.typeMask, lmc.typeMask)) { continue; }
			if (!IS_BINARY_SUBSET(moduleClass.terrMask, lmc.terrMask)) { continue; }
			if (!IS_BINARY_SUBSET(moduleClass.weapMask, lmc.weapMask)) { continue; }
			if (!IS_BINARY_SUBSET(moduleClass.roleMask, lmc.roleMask)) { continue; }

			// NOTE: what if multiple module-classes are suitable?
			moduleState = lmsv[priority];
			break;
		}

		// do NOT cache LuaModule*'s, each AIGroup* must have a unique instance
		// luaModules[moduleClass] = std::vector<LuaModule*>(LuaModule::LUAMODULE_NUM_PRIORITIES, NULL);
		// luaModules[moduleClass][priority] = module;
	}

	// NOTE:
	//   moduleState can be NULL, which means we have a group of units
	//   of type <def> whose class-mask does not map to *any* registered
	//   Lua script (for any priority-level)
	module->SetModuleClass(moduleClass);
	module->SetModuleState(moduleState);
	module->SetPriority(priority);

	return module;
}

lua_State* LuaModuleLoader::LoadLuaModule(const std::string& luaScript) {
	AIHelper* aih = AIHelper::GetActiveInstance();
	IAICallback* rcb = aih->GetCallbackHandler();

	lua_State* luaState = lua_open();
	luaL_openlibs(luaState);

	int loadErr = 0;   // 0 | LUA_ERRFILE | LUA_ERRSYNTAX | LUA_ERRMEM
	int callErr = 0;   // 0 | LUA_ERRRUN  | LUA_ERRMEM    | LUA_ERRERR

	if ((loadErr = luaL_loadfile(luaState, luaScript.c_str())) != 0 || (callErr = lua_pcall(luaState, 0, 0, 0)) != 0) {
		LOG_ERROR(std::string(lua_tostring(luaState, -1)) + "\n");
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



LuaModuleLoader::LuaModuleLoader() {
	AIHelper* aih = AIHelper::GetActiveInstance();
	IAICallback* rcb = aih->GetCallbackHandler();

	const std::string defModuleDirRel = AI_LUA_DIR + "def/";
	const std::string modModuleDirRel = AI_LUA_DIR + "mod/"; // TODO: use modShortName
	const std::string defModuleDirAbs = util::GetAbsFileName(rcb, defModuleDirRel, true);
	const std::string modModuleDirAbs = util::GetAbsFileName(rcb, modModuleDirRel, true);

	std::vector<std::string> defModuleFiles;
	std::vector<std::string> modModuleFiles;
	std::set<std::string> moduleFiles;

	LOG_BASIC("[LuaModuleLoader] loading custom Lua scripts from " << modModuleDirAbs << "\n");

	if (util::GetFilesInDir(modModuleDirAbs, modModuleFiles) == 0) {
		LOG_BASIC("\tfound " << modModuleFiles.size() << " custom scripts\n");

		for (unsigned int i = 0;i < modModuleFiles.size(); i++) {
			LOG_BASIC("\t\tloading " << modModuleFiles[i] << "\n");

			moduleFiles.insert(modModuleFiles[i]);
		}
	}

	LOG_BASIC("[LuaModuleLoader] loading default Lua scripts from " << defModuleDirAbs << "\n");

	if (util::GetFilesInDir(defModuleDirAbs, defModuleFiles) == 0) {
		LOG_BASIC("\tfound " << defModuleFiles.size() << " default scripts\n");

		for (unsigned int i = 0;i < defModuleFiles.size(); i++) {
			if (moduleFiles.find(defModuleFiles[i]) == moduleFiles.end()) {
				LOG_BASIC("\t\tloading " << defModuleFiles[i] << "\n");

				// default module is not overridden, load it
				moduleFiles.insert(defModuleFiles[i]);
			}
		}
	}

	for (std::set<std::string>::const_iterator it = moduleFiles.begin(); it != moduleFiles.end(); ++it) {
		lua_State* moduleState = LoadLuaModule(*it);

		if (moduleState == NULL) {
			// bad script
			continue;
		}

		LuaModule::LuaModuleClass moduleClass;
		unsigned int modulePriority = LuaModule::LUAMODULE_NUM_PRIORITIES;

		// note: asumes <inArgs> is 0
		#define CALL_LUA_FUNC_BEG(L, name, inArgs, outArgs) \
			MAI_ASSERT(lua_gettop(L) == 0);                 \
			lua_getglobal(L, name);                         \
			if (lua_isfunction(L, -1)) {                    \
				lua_call(moduleState, inArgs, outArgs);

		#define CALL_LUA_FUNC_END(L, n)     \
				lua_pop(L, n);              \
			} else {                        \
				lua_pop(L, 1);              \
			}                               \
			MAI_ASSERT(lua_gettop(L) == 0);

		CALL_LUA_FUNC_BEG(moduleState, "GetPriority", 0, 1);
			modulePriority = lua_tointeger(moduleState, -1);
		CALL_LUA_FUNC_END(moduleState, 1);

		CALL_LUA_FUNC_BEG(moduleState, "GetClassMask", 0, 4);
			moduleClass.typeMask = luaL_checkint(moduleState, -4);
			moduleClass.terrMask = luaL_checkint(moduleState, -3);
			moduleClass.weapMask = luaL_checkint(moduleState, -2);
			moduleClass.roleMask = luaL_checkint(moduleState, -1);
		CALL_LUA_FUNC_END(moduleState, 4);


		if (modulePriority >= LuaModule::LUAMODULE_NUM_PRIORITIES) {
			// illegal priority, can't load this module
			continue;
		}

		if (luaModuleStates.find(moduleClass) == luaModuleStates.end()) {
			luaModuleStates[moduleClass] = std::vector<lua_State*>(LuaModule::LUAMODULE_NUM_PRIORITIES, NULL);
			luaModuleStates[moduleClass][modulePriority] = moduleState;
		} else {
			if (luaModuleStates[moduleClass][modulePriority] != NULL) {
				// two or more .lua scripts have the same class-mask _and_
				// the same priority, so we don't require a new lua_State*
				lua_close(moduleState);
			} else {
				luaModuleStates[moduleClass][modulePriority] = moduleState;
			}
		}
	}
}

LuaModuleLoader::~LuaModuleLoader() {
	// close all cached unique Lua states
	typedef std::map<LuaModule::LuaModuleClass, std::vector<lua_State*> > LuaStateMap;
	typedef std::map<LuaModule::LuaModuleClass, std::vector<lua_State*> >::iterator LuaStateMapIt;

	for (LuaStateMapIt mit = luaModuleStates.begin(); mit != luaModuleStates.end(); mit++) {
		std::vector<lua_State*>& luaStateVec = mit->second;

		for (std::vector<lua_State*>::iterator vit = luaStateVec.begin(); vit != luaStateVec.end(); ++vit) {
			lua_close(*vit);
		}

		luaStateVec.clear();
	}

	luaModuleStates.clear();
}
