#include <sstream>

#include "./AILuaHeaders.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaCallBackHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"
#include "../utils/Debugger.hpp"

LuaModule::LuaModule(): 
	isValid(false),
	moduleState(NULL),
	moduleGroup(NULL),
	maxGroupSize(0),
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

		static const unsigned int NUM_CALLINS = 5;
		static const char* callIns[NUM_CALLINS] = {
			"GetName",
			"CanRun",
			"Update",
			"AddUnit",
			"DelUnit",
		};

		for (unsigned int n = 0; n < NUM_CALLINS; n++) {
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
	std::string ret;

	if (isValid) {
		LuaCallBackHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetName");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isstring(moduleState, -1));
		ret = lua_tostring(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}

bool LuaModule::CanRun() {
	bool ret = false;

	if (isValid) {
		LuaCallBackHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "CanRun");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		ret = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}

bool LuaModule::Update() {
	bool ret = false;

	if (isValid) {
		LuaCallBackHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "Update");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		ret = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallBackHandler::SetActiveModule(NULL);
	}

	return ret;
}



void LuaModule::AddUnit(unsigned int unitID) {
	if (isValid) {
		LuaCallBackHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "AddUnit");
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 1, 0);
		lua_pop(moduleState, 1);

		LuaCallBackHandler::SetActiveModule(NULL);
	}
}

void LuaModule::DelUnit(unsigned int unitID) {
	if (isValid) {
		LuaCallBackHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "DelUnit");
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 1, 0);
		lua_pop(moduleState, 1);

		LuaCallBackHandler::SetActiveModule(NULL);
	}
}


std::ostream& operator << (std::ostream& out, rcLuaModule module) {
	out << "Module{name: ";
	// out << module.GetName(); FIXME
	out << ", priority: ";
	out << module.GetPriority();
	out << ", valid: ";
	out << module.IsValid();
	out << ", moduleclass: [";
	out << module.GetModuleClass();
	out << "]}";

	return out;
}

std::ostream& operator << (std::ostream& out, const LuaModule::LuaModuleClass& moduleClass) {
	std::stringstream type_mask_ss; type_mask_ss << "typeMasks[";
	std::stringstream terr_mask_ss; terr_mask_ss << "terrMasks[";
	std::stringstream weap_mask_ss; weap_mask_ss << "weapMasks[";
	std::stringstream role_mask_ss; role_mask_ss << "roleMasks[";

	static cUint32 max_bits = 
		std::max<Uint32>(AIUnitDef::NUM_TYPE_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_TERRAIN_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_WEAPON_MASKS, 
		AIUnitDef::NUM_ROLE_MASKS))
	);

	vUint32 indent(4, 0);

	for (Uint32 i = 0; i < max_bits; i++)
	{
		cUint32 mask = (1 << i);

		if (mask & moduleClass.typeMask)
		{
			type_mask_ss << AIUnitDef::GetTypeMaskName(mask) << "|";
			indent[0] = 1;
		}

		if (mask & moduleClass.terrMask)
		{
			terr_mask_ss << AIUnitDef::GetTerrainMaskName(mask) << "|";
			indent[1] = 1;
		}

		if (mask & moduleClass.weapMask)
		{
			weap_mask_ss << AIUnitDef::GetWeaponMaskName(mask) << "|";
			indent[2] = 1;
		}

		if (mask & moduleClass.roleMask)
		{
			role_mask_ss << AIUnitDef::GetRoleMaskName(mask) << "|";
			indent[3] = 1;
		}
	}

	out << (type_mask_ss.str()).substr(0, type_mask_ss.str().length() - indent[0]) << "], ";
	out << (terr_mask_ss.str()).substr(0, terr_mask_ss.str().length() - indent[1]) << "], ";
	out << (weap_mask_ss.str()).substr(0, weap_mask_ss.str().length() - indent[2]) << "], ";
	out << (role_mask_ss.str()).substr(0, role_mask_ss.str().length() - indent[3]) << "]";

	return out;
}
