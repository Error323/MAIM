#include <sstream>

#include "./AILuaHeaders.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaCallOutHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"
#include "../utils/Debugger.hpp"

LuaModule::LuaModule(): 
	isValid(false),
	moduleState(NULL),
	moduleGroup(NULL),
	priority(LUAMODULE_NUM_PRIORITIES)
{
}

// can be called multiple times over the course
// of a module's lifetime; underlying Lua script
// code that is executed depends on the current
// moduleState
bool LuaModule::SetModuleState(lua_State* L) {
	moduleState = L;
	isValid = true;

	if (moduleState != NULL) {
		MAI_ASSERT(lua_gettop(moduleState) == 0);

		static const Uint32 NUM_CALLINS = 8;
		static const char* callIns[NUM_CALLINS] = {
			"GetMinGroupSize",
			"GetMaxGroupSize",
			"GetName",
			"CanRun",
			"Update",
			"CanAddUnit",
			"AddUnit",
			"DelUnit",
		};

		for (Uint32 n = 0; n < NUM_CALLINS; n++) {
			lua_getglobal(moduleState, callIns[n]);
			isValid = isValid && lua_isfunction(moduleState, -1);
			lua_pop(moduleState, 1);
		}

		MAI_ASSERT(lua_gettop(moduleState) == 0);
	} else {
		isValid = false;
	}

	LOG_BASIC("[LuaModule::SetModuleState()] isValid: " << isValid << "\n");
	return isValid;
}

void LuaModule::Release() {
	if (moduleState != NULL) {
		MAI_ASSERT(lua_gettop(moduleState) == 0);
	}

	moduleState = NULL;
	moduleGroup = NULL;

	ObjectFactory<LuaModule>::Release(this);
}



std::string LuaModule::GetName() {
	std::string name;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetName");
		lua_call(moduleState, 0, 1);
			MAI_ASSERT(lua_isstring(moduleState, -1));
		name = lua_tostring(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return name;
}

Uint32 LuaModule::GetMinGroupSize(Uint32 groupID) {
	Uint32 minGroupSize = 1;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetMinGroupSize");
		lua_pushnumber(moduleState, groupID);
		lua_call(moduleState, 1, 1);
		MAI_ASSERT(lua_isnumber(moduleState, -1));
		minGroupSize = lua_tonumber(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return minGroupSize;
}

Uint32 LuaModule::GetMaxGroupSize(Uint32 groupID) {
	Uint32 maxGroupSize = -1;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetMaxGroupSize");
		lua_pushnumber(moduleState, groupID);
		lua_call(moduleState, 1, 1);
		MAI_ASSERT(lua_isnumber(moduleState, -1));
		maxGroupSize = lua_tonumber(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return maxGroupSize;
}



bool LuaModule::CanRun(Uint32 groupID) {
	bool canRun = false;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "CanRun");
		lua_pushnumber(moduleState, groupID);
		lua_call(moduleState, 1, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		canRun = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return canRun;
}

bool LuaModule::Update(Uint32 groupID) {
	bool update = false;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "Update");
		lua_pushnumber(moduleState, groupID);
		lua_call(moduleState, 1, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		update = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return update;
}



bool LuaModule::CanAddUnit(Uint32 groupID, Uint32 unitID) {
	bool canAddUnit = false;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "CanAddUnit");
		lua_pushnumber(moduleState, groupID);
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 2, 1);
		canAddUnit = (lua_isboolean(moduleState, -1) && lua_toboolean(moduleState, -1));
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return canAddUnit;
}

void LuaModule::AddUnit(Uint32 groupID, Uint32 unitID) {
	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "AddUnit");
		lua_pushnumber(moduleState, groupID);
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 2, 0);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}
}

void LuaModule::DelUnit(Uint32 groupID, Uint32 unitID) {
	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "DelUnit");
		lua_pushnumber(moduleState, groupID);
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 2, 0);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}
}


std::ostream& operator << (std::ostream& out, rcLuaModule module) {
	out << "Module{name: ";
	// out << module.GetName(); FIXME
	out << ", priority: ";
	out << module.GetPriority();
	out << ", valid: ";
	out << module.IsValid();
	out << ", class: [";
	out << module.GetUnitDefClass();
	out << "]}";

	return out;
}
