#include <string>
#include <fstream>

#include "Sim/Units/CommandAI/Command.h"
#include "LegacyCpp/IAICallback.h"

#include "./AILuaHeaders.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaCallOutHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../main/AIDefines.hpp"
#include "../utils/Debugger.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Util.hpp"



LuaModule* LuaModuleLoader::GetModule(const AIUnitDef* def, unsigned int priority) {
	lua_State* moduleState = NULL;
	LuaModule* module = NULL;

	const AIUnitDef::AIUnitDefClass& unitDefClass = def->unitDefClass;

	LOG_BASIC(
		"[GetModule] loading module for UnitDef " << def->GetID() << " "
		"(\"" << def->GetName() << "\") with priority " << priority << "\n"
	);
	LOG_BASIC("\tUnitDefClass type-mask: " << unitDefClass.typeMask << "\n");
	LOG_BASIC("\tUnitDefClass terr-mask: " << unitDefClass.terrMask << "\n");
	LOG_BASIC("\tUnitDefClass weap-mask: " << unitDefClass.weapMask << "\n");
	LOG_BASIC("\tUnitDefClass role-mask: " << unitDefClass.roleMask << "\n");

	if (luaModules.find(unitDefClass) != luaModules.end()) {
		// module = luaModules[unitDefClass][priority];
		// moduleState = luaModuleStates[unitDefClass][priority];
	} else {
		module = ObjectFactory<LuaModule>::Instance();

		typedef std::map<AIUnitDef::AIUnitDefClass, std::vector<lua_State*> > LuaStateMap;
		typedef std::map<AIUnitDef::AIUnitDefClass, std::vector<lua_State*> >::iterator LuaStateMapIt;

		LOG_BASIC("\tnumber of loaded module states: " << luaModuleStates.size() << "\n");

		// sort all matching modules by ascending order of #one-bits
		std::map<AIUnitDef::AIUnitDefClass, lua_State*> matchingStates;

		for (LuaStateMapIt it = luaModuleStates.begin(); it != luaModuleStates.end(); ++it) {
			const AIUnitDef::AIUnitDefClass& lmc = it->first;
			const std::vector<lua_State*>& lmsv = it->second;

			LOG_BASIC("\t\tchecking for binary match with module-class LMC\n");
			LOG_BASIC("\t\t\tLMC type-mask: " << lmc.typeMask << "\n");
			LOG_BASIC("\t\t\tLMC terr-mask: " << lmc.terrMask << "\n");
			LOG_BASIC("\t\t\tLMC weap-mask: " << lmc.weapMask << "\n");
			LOG_BASIC("\t\t\tLMC role-mask: " << lmc.roleMask << "\n");

			// check if <def>'s module-class is suited for this module
			if (!util::IsBinaryMatch(unitDefClass, lmc)) {
				LOG_BASIC("\t\tmodule-class LMC incompatible\n");
				continue;
			}

			// NOTE:
			//     this ASSUMES each module presents _exactly_
			//     the same class-mask for each priority-level
			matchingStates[lmc] = lmsv[priority];
		}

		if (!matchingStates.empty()) {
			// pick the matching class with the fewest total
			// amount of one-bits set (ie. the most specific
			// mask); FIXME does not make much more sense
			moduleState = (matchingStates.begin())->second;
		}

		// do NOT cache LuaModule*'s, each AIGroup* must have a unique instance
		// luaModules[unitDefClass] = std::vector<LuaModule*>(LuaModule::LUAMODULE_NUM_PRIORITIES, NULL);
		// luaModules[unitDefClass][priority] = module;
	}

	// NOTE:
	//   moduleState can be NULL, which means we have a group of units
	//   of type <def> whose class-mask does not map to *any* registered
	//   Lua script (for any priority-level) ==> module will be invalid
	module->SetUnitDefClass(unitDefClass);
	module->SetModuleState(moduleState);
	module->SetPriority(priority);

	LOG_BASIC("[GetModule] is valid module: " << module->IsValid() << "\n");
	return module;
}

lua_State* LuaModuleLoader::LoadLuaModule(const std::string& luaScript, std::string* loadError) {
	lua_State* luaState = lua_open();
	luaL_openlibs(luaState);

	int loadErr = 0;   // 0 | LUA_ERRFILE | LUA_ERRSYNTAX | LUA_ERRMEM
	int callErr = 0;   // 0 | LUA_ERRRUN  | LUA_ERRMEM    | LUA_ERRERR

	if ((loadErr = luaL_loadfile(luaState, luaScript.c_str())) != 0 || (callErr = lua_pcall(luaState, 0, 0, 0)) != 0) {
		*loadError = std::string(lua_tostring(luaState, -1));

		lua_pop(luaState, 1);
		lua_close(luaState);
		return NULL;
	} else {
		// register the callouts for this state
		MAI_ASSERT(lua_gettop(luaState) == 0);

		// AICallOutsTbl = {}
		//   AICallOutsTbl["CallOutSubTbl"] = CallOutSubTbl
		// _G["AICallOuts"] = AICallOutsTbl
		lua_newtable(luaState);
			LuaCallOutHandler::RegisterFunctions(luaState);
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
		lua_setglobal(luaState, "AIModuleClassConstsTbl");
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

	// NOTE: use for globals/{def/, mod/} as well?
	cString defModuleDirRel = AI_LUA_DIR + "groups/" + "def/";
	cString modModuleDirRel = AI_LUA_DIR + "groups/" + rcb->GetModShortName();

	// Modules get installed in the read-only directory, so load them from there
	cString defModuleDirAbs = util::GetAbsFileName(rcb, defModuleDirRel, NULL, true);
	cString modModuleDirAbs = util::GetAbsFileName(rcb, modModuleDirRel, NULL, true);

	vString defModuleFiles;
	vString modModuleFiles;
	std::map<std::string, std::string> moduleFiles;

	LOG_BASIC("[LuaModuleLoader] loading custom Lua scripts from \"" << modModuleDirAbs << "\"\n");

	if (util::GetFilesInDir(modModuleDirAbs, modModuleFiles) == 0) {
		LOG_BASIC("\tnumber of custom Lua scripts found: " << modModuleFiles.size() << "\n");

		for (unsigned int i = 0;i < modModuleFiles.size(); i++) {
			LOG_BASIC("\t\tadding custom Lua script \"" << (modModuleDirAbs + modModuleFiles[i]) << "\" to load-queue\n");

			moduleFiles[modModuleFiles[i]] = modModuleDirAbs;
		}
	}

	LOG_BASIC("[LuaModuleLoader] loading default Lua scripts from \"" << defModuleDirAbs << "\"\n");

	if (util::GetFilesInDir(defModuleDirAbs, defModuleFiles) == 0) {
		LOG_BASIC("\tnumber of default Lua scripts found: " << defModuleFiles.size() << "\n");

		for (unsigned int i = 0; i < defModuleFiles.size(); i++) {
			if (moduleFiles.find(defModuleFiles[i]) == moduleFiles.end()) {
				LOG_BASIC("\t\tadding default Lua script \"" << (defModuleDirAbs + defModuleFiles[i]) << "\" to load-queue\n");

				// default module is not overridden, queue it for loading
				moduleFiles[defModuleFiles[i]] = defModuleDirAbs;
			} else {
				LOG_BASIC("\t\tignoring overriden default Lua script \"" << (defModuleDirAbs + defModuleFiles[i]) << "\"\n");
			}
		}
	}

	LOG_BASIC("[LuaModuleLoader] number of Lua scripts in load-queue: " << moduleFiles.size() << "\n");

	for (std::map<std::string, std::string>::const_iterator it = moduleFiles.begin(); it != moduleFiles.end(); ++it) {
		std::string luaScript = it->second + it->first;
		std::string loadError = "";

		LOG_BASIC("\tloading Lua script \"" << luaScript << "\"\n");

		lua_State* moduleState = LoadLuaModule(luaScript, &loadError);

		if (moduleState == NULL) {
			LOG_ERROR("\t\tload failure: \"" << loadError << "\"\n");
			continue;
		}

		AIUnitDef::AIUnitDefClass unitDefClass;

		Uint32 modulePriority = LuaModule::LUAMODULE_NUM_PRIORITIES;
		Uint32 moduleGroupSize = -1;

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
			modulePriority = luaL_checkint(moduleState, -1);
		CALL_LUA_FUNC_END(moduleState, 1);

		CALL_LUA_FUNC_BEG(moduleState, "GetClass", 0, 4);
			unitDefClass.typeMask = luaL_checkint(moduleState, -4);
			unitDefClass.terrMask = luaL_checkint(moduleState, -3);
			unitDefClass.weapMask = luaL_checkint(moduleState, -2);
			unitDefClass.roleMask = luaL_checkint(moduleState, -1);
		CALL_LUA_FUNC_END(moduleState, 4);


		if (modulePriority >= LuaModule::LUAMODULE_NUM_PRIORITIES) {
			LOG_ERROR("\t\tinvalid module priority: " << modulePriority << "\n");
			lua_close(moduleState);
			continue;
		}

		LOG_BASIC("\tloaded Lua script \"" << luaScript << "\"\n");
		LOG_BASIC("\t\tscript priority: " << modulePriority << "\n");
		LOG_BASIC("\t\tscript type-mask: " << unitDefClass.typeMask << "\n");
		LOG_BASIC("\t\tscript terr-mask: " << unitDefClass.terrMask << "\n");
		LOG_BASIC("\t\tscript weap-mask: " << unitDefClass.weapMask << "\n");
		LOG_BASIC("\t\tscript role-mask: " << unitDefClass.roleMask << "\n");

		if (luaModuleStates.find(unitDefClass) == luaModuleStates.end()) {
			luaModuleStates[unitDefClass] = std::vector<lua_State*>(LuaModule::LUAMODULE_NUM_PRIORITIES, NULL);
			luaModuleStates[unitDefClass][modulePriority] = moduleState;
		} else {
			if (luaModuleStates[unitDefClass][modulePriority] != NULL) {
				// two or more .lua scripts have the same class-mask _and_
				// the same priority, so we don't require a new lua_State*
				lua_close(moduleState);
			} else {
				luaModuleStates[unitDefClass][modulePriority] = moduleState;
			}
		}
	}
}

LuaModuleLoader::~LuaModuleLoader() {
	// close all cached unique Lua states
	typedef std::map<AIUnitDef::AIUnitDefClass, std::vector<lua_State*> > LuaStateMap;
	typedef std::map<AIUnitDef::AIUnitDefClass, std::vector<lua_State*> >::iterator LuaStateMapIt;

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
