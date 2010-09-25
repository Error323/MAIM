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
	haveGetName(false),
	haveCanRun(false),
	haveUpdate(false),
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

	if (moduleState != NULL) {
		MAI_ASSERT(lua_gettop(moduleState) == 0);

		lua_getglobal(moduleState, "GetName");
			haveGetName = lua_isfunction(moduleState, -1);
		lua_pop(moduleState, 1);
		lua_getglobal(moduleState, "CanRun");
			haveCanRun = lua_isfunction(moduleState, -1);
		lua_pop(moduleState, 1);
		lua_getglobal(moduleState, "Update");
			haveUpdate = lua_isfunction(moduleState, -1);
		lua_pop(moduleState, 1);

		isValid = (haveGetName && haveCanRun && haveUpdate);
		MAI_ASSERT(lua_gettop(moduleState) == 0);
	} else {
		isValid     = false;
		haveGetName = false;
		haveCanRun  = false;
		haveUpdate  = false;
	}

	LOG_BASIC("[LuaModule::SetModuleState()]\n");
	LOG_BASIC("\tHaveGetName(): " << (HaveGetName()) << "\n");
	LOG_BASIC("\tHaveCanRun():  " << (HaveCanRun())  << "\n");
	LOG_BASIC("\tHaveUpdate():  " << (HaveUpdate()) << "\n");

	return (isValid);
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

	if (isValid && haveGetName) {
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

	if (isValid && haveCanRun) {
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

	if (isValid && haveUpdate) {
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

std::ostream& operator<<(std::ostream &out, rcLuaModule module) {
	out << "Module{name:";
	//out << module.GetName(); FIXME
	out << ", priority:";
	out << module.GetPriority();
	out << ", valid:";
	out << module.IsValid();
	out << ", moduleclass:[";
	out << module.GetModuleClass();
	out << "]}";

	return out;
}

std::ostream& operator<<(std::ostream &out, const LuaModule::LuaModuleClass& moduleClass) {
	std::stringstream types; types << "types[";
	std::stringstream terrains; terrains << "terrains[";
	std::stringstream weapons; weapons << "weapons[";
	std::stringstream roles; roles << "roles[";

	static cUint32 max_bits = 
		std::max<Uint32>(AIUnitDef::NUM_TYPE_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_TERRAIN_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_WEAPON_MASKS, 
		AIUnitDef::NUM_ROLE_MASKS))
	);

	int a, b, c, d;
	a = b = c = d = 0;
	for (Uint32 i = 0; i < max_bits; i++)
	{
		Uint32 mask = (1 << i);
		if (mask&moduleClass.typeMask)
		{
			types << AIUnitDef::GetTypeMaskName(mask) << "|";
			a = 1;
		}
		if (mask&moduleClass.terrMask)
		{
			terrains << AIUnitDef::GetTerrainMaskName(mask) << "|";
			b = 1;
		}
		if (mask&moduleClass.weapMask)
		{
			weapons << AIUnitDef::GetWeaponMaskName(mask) << "|";
			c = 1;
		}
		if (mask&moduleClass.roleMask)
		{
			roles << AIUnitDef::GetRoleMaskName(mask) << "|";
			d = 1;
		}
	}

	out << types.str().substr(0,types.str().length()-a) << "], ";
	out << terrains.str().substr(0,terrains.str().length()-b) << "], ";
	out << weapons.str().substr(0,weapons.str().length()-c) << "], ";
	out << roles.str().substr(0, roles.str().length()-d) << "]";

	return out;
}
