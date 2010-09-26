#include <sstream>

#include "./AILuaHeaders.hpp"
#include "./AILuaModule.hpp"
#include "./AILuaModuleLoader.hpp"
#include "./AILuaCallOutHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../units/AIUnitDef.hpp"
#include "../utils/ObjectFactory.hpp"
#include "../utils/Logger.hpp"
#include "../utils/Util.hpp"
#include "../utils/Debugger.hpp"

bool LuaModule::LuaModuleClass::operator < (rcLuaModuleClass mc) const {
	// sort by typeMask, then by terrMask, then by weapMask, then by roleMask
	// note that we only look at the number of 1-bits, not which bits are set
	if (util::CountOneBits(typeMask) < util::CountOneBits(mc.typeMask)) { return true; }
	if (util::CountOneBits(terrMask) < util::CountOneBits(mc.terrMask)) { return true; }
	if (util::CountOneBits(weapMask) < util::CountOneBits(mc.weapMask)) { return true; }
	if (util::CountOneBits(roleMask) < util::CountOneBits(mc.roleMask)) { return true; }
	return false;
}

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

		static const Uint32 NUM_CALLINS = 7;
		static const char* callIns[NUM_CALLINS] = {
			"GetMinGroupSize",
			"GetMaxGroupSize",
			"GetName",
			"CanRun",
			"Update",
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

Uint32 LuaModule::GetMinGroupSize() {
	Uint32 minGroupSize = 1;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetMinGroupSize");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isnumber(moduleState, -1));
		minGroupSize = lua_tonumber(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return minGroupSize;
}

Uint32 LuaModule::GetMaxGroupSize() {
	Uint32 maxGroupSize = -1;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "GetMaxGroupSize");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isnumber(moduleState, -1));
		maxGroupSize = lua_tonumber(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return maxGroupSize;
}



bool LuaModule::CanRun() {
	bool canRun = false;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "CanRun");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		canRun = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return canRun;
}

bool LuaModule::Update() {
	bool update = false;

	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "Update");
		lua_call(moduleState, 0, 1);
		MAI_ASSERT(lua_isboolean(moduleState, -1));
		update = lua_toboolean(moduleState, -1);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}

	return update;
}



void LuaModule::AddUnit(Uint32 unitID) {
	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "AddUnit");
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 1, 0);
		lua_pop(moduleState, 1);

		LuaCallOutHandler::SetActiveModule(NULL);
	}
}

void LuaModule::DelUnit(Uint32 unitID) {
	if (isValid) {
		LuaCallOutHandler::SetActiveModule(this);

		lua_getglobal(moduleState, "DelUnit");
		lua_pushnumber(moduleState, unitID);
		lua_call(moduleState, 1, 0);
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
