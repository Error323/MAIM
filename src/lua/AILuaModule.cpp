#include <sstream>

#include "./AILuaHeaders.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaCallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"
#include "../utils/Debugger.hpp"

LuaModule::LuaModule(): 
	isValid(false), 
	haveGetName(false), 
	haveCanRun(false), 
	haveUpdate(false), 
	luaState(NULL), 
	maxGroupSize(0),
	priority(MODULE_PRIORITY_COUNT)
{
}

// can be called multiple times over the course
// of a module's lifetime; underlying Lua script
// code that is executed depends on the current
// luaState
bool LuaModule::LoadState(const std::string& moduleName) {
	luaState = AIHelper::GetActiveInstance()->luaModuleLoader->LoadLuaModule(moduleName);

	if (luaState != NULL) {
		MAI_ASSERT(lua_gettop(luaState) == 0);

		lua_getglobal(luaState, "GetName");
			haveGetName = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);
		lua_getglobal(luaState, "CanRun");
			haveCanRun = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);
		lua_getglobal(luaState, "Update");
			haveUpdate = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);

		isValid = (haveGetName && haveCanRun && haveUpdate);
		MAI_ASSERT(lua_gettop(luaState) == 0);
	} else {
		isValid     = false;
		haveGetName = false;
		haveCanRun  = false;
		haveUpdate  = false;
	}

	LOG_BASIC(AIHelper::GetActiveInstance()->logger, "[LuaModule::LoadState(" << moduleName << ")]\n");
	LOG_BASIC(AIHelper::GetActiveInstance()->logger, "\tHaveGetName(): " << (HaveGetName()) << "\n");
	LOG_BASIC(AIHelper::GetActiveInstance()->logger, "\tHaveCanRun():  " << (HaveCanRun())  << "\n");
	LOG_BASIC(AIHelper::GetActiveInstance()->logger, "\tHaveUpdate():  " << (HaveUpdate()) << "\n");

	return (isValid);
}

void LuaModule::Release() {
	MAI_ASSERT(lua_gettop(luaState) == 0);
	ObjectFactory<LuaModule>::Release(this);
}



std::string LuaModule::GetName() {
	std::string ret;

	if (isValid && haveGetName) {
		LuaCallBackHandler::SetActiveModule(this);
		lua_getglobal(luaState, "GetName");
		lua_call(luaState, 0, 1);
		MAI_ASSERT(lua_isstring(luaState, -1));
		ret = lua_tostring(luaState, -1);
		lua_pop(luaState, 1);
		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}

bool LuaModule::CanRun() {
	bool ret = false;

	if (isValid && haveCanRun) {
		LuaCallBackHandler::SetActiveModule(this);
		lua_getglobal(luaState, "CanRun");
		lua_call(luaState, 0, 1);
		MAI_ASSERT(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}

bool LuaModule::Update() {
	bool ret = false;

	if (isValid && haveUpdate) {
		LuaCallBackHandler::SetActiveModule(this);
		lua_getglobal(luaState, "Update");
		lua_call(luaState, 0, 1);
		MAI_ASSERT(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}



bool LuaModule::IsSuited(unsigned unitTypeMasks, unsigned unitTerrainMasks, unsigned unitWeaponMasks, unsigned unitMoveMasks) {
	const bool a = util::IsBinarySubset(moduleTypeMasks,    unitTypeMasks);
	const bool b = util::IsBinarySubset(moduleTerrainMasks, unitTerrainMasks);
	const bool c = util::IsBinarySubset(moduleWeaponMasks,  unitWeaponMasks);
	const bool d = util::IsBinarySubset(moduleMoveMasks,    unitMoveMasks);
	return (a && b && c && d);
}
