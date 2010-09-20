#include <sstream>
#include <cassert>

#include "./LuaModule.hpp"
#include "../lua/LuaModuleLoader.hpp"
#include "../lua/LuaAICallBackHandler.hpp"
#include "../main/AILua.hpp"
#include "../main/AIHelper.hpp"
#include "../factories/Factory.hpp"
#include "../utils/Logger.hpp"

LuaModule::LuaModule(): isValid(false), haveGetName(false), haveCanRun(false), haveUpdate(false), luaState(NULL) {
}

// can be called multiple times over the course
// of a module's lifetime; underlying Lua script
// code that is executed depends on the current
// luaState
bool LuaModule::LoadState(const std::string& moduleName) {
	luaState = aih->luaModuleLoader->LoadLuaModule(moduleName);

	if (luaState != NULL) {
		assert(lua_gettop(luaState) == 0);

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
		assert(lua_gettop(luaState) == 0);
	} else {
		isValid     = false;
		haveGetName = false;
		haveCanRun  = false;
		haveUpdate  = false;
	}

	LOG_BASIC(aih->logger, "[LuaModule::LoadState(" << moduleName << ")]\n");
	LOG_BASIC(aih->logger, "\tHaveGetName(): " << (HaveGetName()) << "\n");
	LOG_BASIC(aih->logger, "\tHaveCanRun():  " << (HaveCanRun())  << "\n");
	LOG_BASIC(aih->logger, "\tHaveUpdate():  " << (HaveUpdate()) << "\n");

	return (isValid);
}

void LuaModule::Release() {
	assert(lua_gettop(luaState) == 0);
	Factory<LuaModule>::Release(this);
}



std::string LuaModule::GetName() {
	std::string ret;

	if (isValid && haveGetName) {
		LuaAICallBackHandler::SetModule(this);
		lua_getglobal(luaState, "GetName");
		lua_call(luaState, 0, 1);
		assert(lua_isstring(luaState, -1));
		ret = lua_tostring(luaState, -1);
		lua_pop(luaState, 1);
		LuaAICallBackHandler::SetModule(NULL);
	}

	return ret;
}

bool LuaModule::CanRun() {
	bool ret = false;

	if (isValid && haveCanRun) {
		LuaAICallBackHandler::SetModule(this);
		lua_getglobal(luaState, "CanRun");
		lua_call(luaState, 0, 1);
		assert(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
		LuaAICallBackHandler::SetModule(NULL);
	}

	return ret;
}
bool LuaModule::Update() {
	bool ret = false;

	if (isValid && haveUpdate) {
		LuaAICallBackHandler::SetModule(this);
		lua_getglobal(luaState, "Update");
		lua_call(luaState, 0, 1);
		assert(lua_isboolean(luaState, -1));
		ret = lua_toboolean(luaState, -1);
		lua_pop(luaState, 1);
		LuaAICallBackHandler::SetModule(NULL);
	}

	return ret;
}
