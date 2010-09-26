#include <string>
#include <fstream>

#include "Sim/Units/CommandAI/Command.h"
#include "LegacyCpp/IAICallback.h"

#include "./AILuaHeaders.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaCallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../main/AIDefines.hpp"
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
	lua_State* luaState = lua_open();
	luaL_openlibs(luaState);

	int loadErr = 0;   // 0 | LUA_ERRFILE | LUA_ERRSYNTAX | LUA_ERRMEM
	int callErr = 0;   // 0 | LUA_ERRRUN  | LUA_ERRMEM    | LUA_ERRERR

	if ((loadErr = luaL_loadfile(luaState, luaScript.c_str())) != 0 || (callErr = lua_pcall(luaState, 0, 0, 0)) != 0) {
		LOG_ERROR(std::string(lua_tostring(luaState, -1)) + "\n");
		lua_pop(luaState, 1);
		return NULL;
	} else {
		// register the callbacks for this state
		MAI_ASSERT(lua_gettop(luaState) == 0);

		// AICallOuts = {}
		lua_newtable(luaState);
			// EcoStateTbl = {}
			lua_pushstring(luaState, "EcoStateTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				lua_pushstring(luaState, "IsStallingMetal");
				lua_pushcfunction(luaState, LuaCallBackHandler::EcoStateCallBacks::IsStallingMetal);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingMetal"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);

				lua_pushstring(luaState, "IsStallingEnergy");
				lua_pushcfunction(luaState, LuaCallBackHandler::EcoStateCallBacks::IsStallingEnergy);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingEnergy"] = func
				MAI_ASSERT(lua_gettop(luaState) == 3);
			// AICallOuts["EcoStateTbl"] = EcoStateTbl
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			// GameMapTbl = {}
			lua_pushstring(luaState, "GameMapTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				// GameMap["GetAmountOfLand"] = func
				lua_pushstring(luaState, "GetAmountOfLand");
				lua_pushcfunction(luaState, LuaCallBackHandler::GameMapCallBacks::GetAmountOfLand);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3);
				MAI_ASSERT(lua_gettop(luaState) == 3);

				// GameMap["GetAmountOfWater"] = func
				lua_pushstring(luaState, "GetAmountOfWater");
				lua_pushcfunction(luaState, LuaCallBackHandler::GameMapCallBacks::GetAmountOfWater);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3);
				MAI_ASSERT(lua_gettop(luaState) == 3);
			// AICallOuts["GameMapTbl"] = GameMapTbl
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			// CommandsTbl = {}
			lua_pushstring(luaState, "CommandsTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				lua_pushstring(luaState, "GiveCommand");
				lua_pushcfunction(luaState, LuaCallBackHandler::CommandCallBacks::GiveCommand);
				MAI_ASSERT(lua_istable(luaState, -3));
				lua_settable(luaState, -3);
				MAI_ASSERT(lua_gettop(luaState) == 3);
			// AICallOuts["CommandsTbl"] = CommandsTbl
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

		// add the AI root table to the global environment
		lua_setglobal(luaState, "AICallOutsTbl");
		MAI_ASSERT(lua_gettop(luaState) == 0);


		#define PUSH_LUA_MASK_CONSTANT(L, c) \
			lua_pushstring(L, #c);           \
			lua_pushnumber(L, AIUnitDef::c); \
			lua_rawset(L, -3);
		#define PUSH_LUA_PRIORITY_CONSTANT(L, c) \
			lua_pushstring(L, #c);               \
			lua_pushnumber(L, LuaModule::c);     \
			lua_rawset(L, -3);
		#define PUSH_LUA_COMMAND_CONSTANT(L, c) \
			lua_pushstring(L, #c);              \
			lua_pushnumber(L, c);               \
			lua_rawset(L, -3);

		lua_newtable(luaState);
			lua_pushstring(luaState, "TypeMasksTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_BUILDER_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_BUILDER_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ASSISTER_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ASSISTER_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_E_PRODUCER_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_E_PRODUCER_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_M_PRODUCER_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_M_PRODUCER_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_E_STORAGE_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_E_STORAGE_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_M_STORAGE_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_M_STORAGE_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_DEFENSE_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_DEFENSE_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_OFFENSE_STATIC);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_OFFENSE_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_INTEL_MOBILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_INTEL_STATIC);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "TerrainMasksTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_LAND);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_WATER_SURFACE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_WATER_SUBMERGED);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_AIR);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "WeaponMasksTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ARMED);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_NUKE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ANTINUKE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_SHIELD);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_STOCKPILE);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_MANUALFIRE);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "RoleMasksTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_SCOUT);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_RAIDER);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ASSAULT);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ARTILLERY);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_ANTIAIR);
				PUSH_LUA_MASK_CONSTANT(luaState, MASK_STRIKER);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);
		lua_setglobal(luaState, "AIModuleClassMaskConstsTbl");
		MAI_ASSERT(lua_gettop(luaState) == 0);

		lua_newtable(luaState);
			PUSH_LUA_PRIORITY_CONSTANT(luaState, LUAMODULE_PRIORITY_EMERGENCY);
			PUSH_LUA_PRIORITY_CONSTANT(luaState, LUAMODULE_PRIORITY_REACTIVE);
			PUSH_LUA_PRIORITY_CONSTANT(luaState, LUAMODULE_PRIORITY_PROACTIVE);
		lua_setglobal(luaState, "AIModulePriorityConstsTbl");
		MAI_ASSERT(lua_gettop(luaState) == 0);

		lua_newtable(luaState);
			lua_pushstring(luaState, "TypesTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_STOP);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_WAIT);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_MOVE);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_ATTACK);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_GUARD);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_REPAIR);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_RECLAIM);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_CLOAK);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_DGUN);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_RESURRECT);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CMD_CAPTURE);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);

			lua_pushstring(luaState, "OptionsTbl");
			lua_newtable(luaState);
			MAI_ASSERT(lua_istable(luaState, -3));
			MAI_ASSERT(lua_istable(luaState, -1));
				PUSH_LUA_COMMAND_CONSTANT(luaState, META_KEY);
				PUSH_LUA_COMMAND_CONSTANT(luaState, DONT_REPEAT);
				PUSH_LUA_COMMAND_CONSTANT(luaState, RIGHT_MOUSE_KEY);
				PUSH_LUA_COMMAND_CONSTANT(luaState, SHIFT_KEY);
				PUSH_LUA_COMMAND_CONSTANT(luaState, CONTROL_KEY);
				PUSH_LUA_COMMAND_CONSTANT(luaState, ALT_KEY);
			lua_settable(luaState, -3);
			MAI_ASSERT(lua_gettop(luaState) == 1);
		lua_setglobal(luaState, "AICommandConstsTbl");
		MAI_ASSERT(lua_gettop(luaState) == 0);

		#undef PUSH_LUA_MASK_CONSTANT
		#undef PUSH_LUA_PRIORITY_CONSTANT
		#undef PUSH_LUA_COMMAND_CONSTANT

		// lua_register(L, name, func) is short-hand macro for
		// lua_pushcfunction(L, func) + lua_setglobal(L, name)
		// do we want any functions available in the global env (_G)?

		return luaState;
	}
}



LuaModuleLoader::LuaModuleLoader() {
	AIHelper* aih = AIHelper::GetActiveInstance();
	IAICallback* rcb = aih->GetCallbackHandler();

	cString defModuleDirRel = AI_LUA_DIR + "def/";
	cString modModuleDirRel = AI_LUA_DIR + rcb->GetModShortName();

	// Modules get installed in the readonly directory, so load them from there
	cString defModuleDirAbs = util::GetAbsFileName(rcb, defModuleDirRel, NULL, true);
	cString modModuleDirAbs = util::GetAbsFileName(rcb, modModuleDirRel, NULL, true);

	vString defModuleFiles;
	vString modModuleFiles;
	std::map<std::string, std::string> moduleFiles;

	LOG_BASIC("[LuaModuleLoader] loading custom Lua scripts from " << modModuleDirAbs << "\n");

	if (util::GetFilesInDir(modModuleDirAbs, modModuleFiles) == 0) {
		LOG_BASIC("\tfound " << modModuleFiles.size() << " custom scripts\n");

		for (unsigned int i = 0;i < modModuleFiles.size(); i++) {
			LOG_BASIC("\t\tadding " << (modModuleDirAbs + modModuleFiles[i]) << "\n");

			moduleFiles[modModuleFiles[i]] = modModuleDirAbs;
		}
	}

	LOG_BASIC("[LuaModuleLoader] loading default Lua scripts from " << defModuleDirAbs << "\n");

	if (util::GetFilesInDir(defModuleDirAbs, defModuleFiles) == 0) {
		LOG_BASIC("\tfound " << defModuleFiles.size() << " default scripts\n");

		for (unsigned int i = 0;i < defModuleFiles.size(); i++) {
			if (moduleFiles.find(defModuleFiles[i]) == moduleFiles.end()) {
				LOG_BASIC("\t\tadding " << (defModuleDirAbs + defModuleFiles[i]) << "\n");

				// default module is not overridden, load it
				moduleFiles[defModuleFiles[i]] = defModuleDirAbs;
			}
		}
	}

	LOG_BASIC("[LuaModuleLoader] found " << moduleFiles.size() << " unique scripts\n");

	for (std::map<std::string, std::string>::const_iterator it = moduleFiles.begin(); it != moduleFiles.end(); ++it) {
		LOG_BASIC("\tloading " << (it->second + it->first) << "\n");

		lua_State* moduleState = LoadLuaModule(it->second + it->first);

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
			LOG_ERROR("\t\t" << it->first << " has an invalid priority " << modulePriority << "\n");
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
			if (*vit != NULL) {
				lua_close(*vit);
			}
		}

		luaStateVec.clear();
	}

	luaModuleStates.clear();
}
