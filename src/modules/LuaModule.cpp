#include <cassert>

#include "./LuaModule.hpp"
#include "../lua/LuaAICallBackHandler.hpp"
#include "../main/AILua.hpp"
#include "../main/AIHelper.hpp"
#include "../utils/Logger.hpp"

LuaModule::LuaModule(): isValid(false), haveGetName(false), haveCanRun(false), haveUpdate(false) {
}

bool LuaModule::Load(const std::string& luaScript) {
	luaState = lua_open();
	luaL_openlibs(luaState);

	int loadErr = 0;   // 0 | LUA_ERRFILE | LUA_ERRSYNTAX | LUA_ERRMEM
	int callErr = 0;   // 0 | LUA_ERRRUN  | LUA_ERRMEM    | LUA_ERRERR

	if ((loadErr = luaL_loadfile(luaState, luaScript.c_str())) != 0 || (callErr = lua_pcall(luaState, 0, 0, 0)) != 0) {
		aih->logger->Log(std::string(lua_tostring(luaState, -1)) + "\n");
		lua_pop(luaState, 1);
	} else {
		isValid = true;

		lua_getglobal(luaState, "GetName");
			haveGetName = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);
		lua_getglobal(luaState, "CanRun");
			haveCanRun = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);
		lua_getglobal(luaState, "Update");
			haveUpdate = lua_isfunction(luaState, -1);
		lua_pop(luaState, 1);

		assert(lua_gettop(luaState) == 0);

		lua_newtable(luaState); // AI = {}
			lua_pushstring(luaState, "EcoState");
			lua_newtable(luaState); // EcoState = {}
			assert(lua_istable(luaState, -3));
			assert(lua_istable(luaState, -1));
				lua_pushstring(luaState, "IsStallingMetal");
				lua_pushcfunction(luaState, LuaAICallBackHandler::EcoStateIsStallingMetal);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingMetal"] = func
				assert(lua_gettop(luaState) == 1);

				lua_pushstring(luaState, "IsStallingEnergy");
				lua_pushcfunction(luaState, LuaAICallBackHandler::EcoStateIsStallingEnergy);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // EcoState["IsStallingEnergy"] = func
				assert(lua_gettop(luaState) == 1);
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
				assert(lua_gettop(luaState) == 1);

				lua_pushstring(luaState, "GetAmountOfWater");
				lua_pushcfunction(luaState, LuaAICallBackHandler::GameMapGetAmountOfWater);
				assert(lua_istable(luaState, -3));
				lua_settable(luaState, -3); // GameMap["GetAmountOfWater"] = func
				assert(lua_gettop(luaState) == 1);
			lua_settable(luaState, -3); // AI["GameMap"] = GameMap
			assert(lua_gettop(luaState) == 1);

		// add the AI root table to the global environment
		lua_setglobal(luaState, "AI");
		assert(lua_gettop(luaState) == 0);

		// lua_register(L, name, func) is short-hand macro for
		// lua_pushcfunction(L, func) + lua_setglobal(L, name)
		// do we want any functions available in the global env?
	}

	return isValid;
}

LuaModule::~LuaModule() {
	lua_close(luaState);
}

// modules are released by groups when the
// group is destroyed (released), but do we
// want this behavior for Lua modules which
// are allocated/released via and cached by
// the LuaModuleLoader?
// ROF now acts as sort-of cache, but still
// allocates new instances when none of the
// old ones are in the free-list
// ie., whenever more than one single group
// is alive at the same time and needs the
// same LuaModule, the ROF will 'new' one
// for each
/*
void LuaModule::Release() {
	ReusableObjectFactory<LuaModule>::Release(this);
}
*/



// note: AModule::GetName() needs to be virtual
std::string LuaModule::GetName() {
	std::string ret;

	if (isValid && haveGetName) {
		lua_getglobal(luaState, "GetName");
		lua_call(luaState, 0, 1);
		assert(lua_isstring(luaState, -1));
		ret = lua_tostring(luaState, -1);
		lua_pop(luaState, 1);
	}

	return ret;
}

bool LuaModule::CanRun() {
	bool ret = false;

	if (isValid && haveCanRun) {
		lua_getglobal(luaState, "CanRun");
		lua_call(luaState, 0, 1);
		assert(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
	}

	return ret;
}
bool LuaModule::Update() {
	bool ret = false;

	if (isValid && haveUpdate) {
		lua_getglobal(luaState, "Update");
		lua_call(luaState, 0, 1);
		assert(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
	}

	return ret;
}
