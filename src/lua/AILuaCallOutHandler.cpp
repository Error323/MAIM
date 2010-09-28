#include "Sim/Units/CommandAI/Command.h"
#include "LegacyCpp/IAICallback.h"

#include "./AILuaCallOutHandler.hpp"
#include "./AILuaHeaders.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../units/AIUnitHandler.hpp"
#include "../utils/Debugger.hpp"

LuaModule* LuaCallOutHandler::activeModule = NULL;

#define REGISTER_LUA_FUNCTION(L, name)   \
	lua_pushstring(L, #name);            \
	lua_pushcfunction(L, name);          \
	MAI_ASSERT(lua_istable(L, -3));      \
	lua_settable(L, -3);                 \
	MAI_ASSERT(lua_gettop(L) == 3);

void LuaCallOutHandler::RegisterFunctions(lua_State* L) {
	SimStateCallOuts::RegisterFunctions(L);
	UnitStateCallOuts::RegisterFunctions(L);
	EcoStateCallOuts::RegisterFunctions(L);
	GameMapCallOuts::RegisterFunctions(L);
	CommandCallOuts::RegisterFunctions(L);
}

void LuaCallOutHandler::SimStateCallOuts::RegisterFunctions(lua_State* L) {
	// SimStateCallOutsTbl = {}
	//   SimStateCallOutsTbl["func"] = func
	// AICallOutsTbl["SimStateCallOutsTbl"] = SimStateCallOutsTbl
	lua_pushstring(L, "SimStateCallOutsTbl");
	lua_newtable(L);
	MAI_ASSERT(lua_istable(L, -3));
	MAI_ASSERT(lua_istable(L, -1));
		REGISTER_LUA_FUNCTION(L, GetInitSimFrame);
		REGISTER_LUA_FUNCTION(L, GetCurrSimFrame);
		REGISTER_LUA_FUNCTION(L, IsPaused);
	lua_settable(L, -3);
	MAI_ASSERT(lua_gettop(L) == 1);
}

void LuaCallOutHandler::UnitStateCallOuts::RegisterFunctions(lua_State* L) {
	// UnitStateCallOutsTbl = {}
	//   UnitStateCallOutsTbl["func"] = func
	// AICallOutsTbl["UnitStateCallOutsTbl"] = UnitStateCallOutsTbl
	lua_pushstring(L, "UnitStateCallOutsTbl");
	lua_newtable(L);
	MAI_ASSERT(lua_istable(L, -3));
	MAI_ASSERT(lua_istable(L, -1));
		REGISTER_LUA_FUNCTION(L, GetUnitPos);
		REGISTER_LUA_FUNCTION(L, GetUnitVel);
		REGISTER_LUA_FUNCTION(L, GetUnitHealth);
	lua_settable(L, -3);
	MAI_ASSERT(lua_gettop(L) == 1);
}

void LuaCallOutHandler::EcoStateCallOuts::RegisterFunctions(lua_State* L) {
	// EcoStateCallOutsTbl = {}
	//   EcoStateCallOutsTbl["func"] = func
	// AICallOutsTbl["EcoStateCallOutsTbl"] = EcoStateCallOutsTbl
	lua_pushstring(L, "EcoStateCallOutsTbl");
	lua_newtable(L);
	MAI_ASSERT(lua_istable(L, -3));
	MAI_ASSERT(lua_istable(L, -1));
		REGISTER_LUA_FUNCTION(L, IsStallingMetal);
		REGISTER_LUA_FUNCTION(L, IsStallingEnergy);
	lua_settable(L, -3);
	MAI_ASSERT(lua_gettop(L) == 1);
}

void LuaCallOutHandler::GameMapCallOuts::RegisterFunctions(lua_State* L) {
	// GameMapCallOutsTbl = {}
	//   GameMapCallOutsTbl["func"] = func
	// AICallOutsTbl["GameMapTbl"] = GameMapCallOutsTbl
	lua_pushstring(L, "GameMapCallOutsTbl");
	lua_newtable(L);
	MAI_ASSERT(lua_istable(L, -3));
	MAI_ASSERT(lua_istable(L, -1));
		REGISTER_LUA_FUNCTION(L, GetAmountOfLand);
		REGISTER_LUA_FUNCTION(L, GetAmountOfWater);
	lua_settable(L, -3);
	MAI_ASSERT(lua_gettop(L) == 1);
}
void LuaCallOutHandler::CommandCallOuts::RegisterFunctions(lua_State* L) {
	// CommandCallOutsTbl = {}
	//   CommandCallOutsTbl["func"] = func
	// AICallOutsTbl["CommandCallOutsTbl"] = CommandCallOutsTbl
	lua_pushstring(L, "CommandCallOutsTbl");
	lua_newtable(L);
	MAI_ASSERT(lua_istable(L, -3));
	MAI_ASSERT(lua_istable(L, -1));
		REGISTER_LUA_FUNCTION(L, GiveCommand);
	lua_settable(L, -3);
	MAI_ASSERT(lua_gettop(L) == 1);
}

#undef REGISTER_LUA_FUNCTION



int LuaCallOutHandler::SimStateCallOuts::GetInitSimFrame(lua_State* L) {
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetInitFrame());
	return 1;
}
int LuaCallOutHandler::SimStateCallOuts::GetCurrSimFrame(lua_State* L) {
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetCurrFrame());
	return 1;
}
int LuaCallOutHandler::SimStateCallOuts::IsPaused(lua_State* L) {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	bool isPaused = false;
	rcb->GetValue(AIVAL_GAME_PAUSED, &isPaused);

	lua_pushboolean(L, isPaused);
	return 1;
}


int LuaCallOutHandler::UnitStateCallOuts::GetUnitPos(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) >= 1);

	// unitID
	if (!lua_isnumber(L, 1)) {
		return 0;
	}

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	const float3& pos = rcb->GetUnitPos(lua_tointeger(L, 1));

	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	lua_pushnumber(L, pos.z);
	return 3;
}
int LuaCallOutHandler::UnitStateCallOuts::GetUnitVel(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) >= 1);

	// unitID
	if (!lua_isnumber(L, 1)) {
		return 0;
	}

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	const float3& vel = rcb->GetUnitVel(lua_tointeger(L, 1));

	lua_pushnumber(L, vel.x);
	lua_pushnumber(L, vel.y);
	lua_pushnumber(L, vel.z);
	return 3;
}
int LuaCallOutHandler::UnitStateCallOuts::GetUnitHealth(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) >= 1);

	// unitID
	if (!lua_isnumber(L, 1)) {
		return 0;
	}

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	lua_pushnumber(L, rcb->GetUnitHealth(lua_tointeger(L, 1)));
	return 1;
}


int LuaCallOutHandler::EcoStateCallOuts::IsStallingMetal(lua_State* L) {
	lua_pushboolean(L, AIHelper::GetActiveInstance()->GetEcoState()->IsStallingMetal());
	return 1;
}
int LuaCallOutHandler::EcoStateCallOuts::IsStallingEnergy(lua_State* L) {
	lua_pushboolean(L, AIHelper::GetActiveInstance()->GetEcoState()->IsStallingEnergy());
	return 1;
}


int LuaCallOutHandler::GameMapCallOuts::GetAmountOfLand(lua_State* L) {
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetGameMap()->GetAmountOfLand());
	return 1;
}

int LuaCallOutHandler::GameMapCallOuts::GetAmountOfWater(lua_State* L) {
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetGameMap()->GetAmountOfWater());
	return 1;
}


int LuaCallOutHandler::CommandCallOuts::GiveCommand(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) >= 2);

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();
	pAIUnitHandler uh = aih->GetAIUnitHandler();

	if (!lua_isnumber(L, 1)) { return 0; } // unitID
	if (!lua_istable(L, 2)) { return 0; } // commandTbl

	const Uint32 unitID = lua_tointeger(L, 1);
	const Uint32 tableIdx = 2; // absolute index

	if (uh == NULL || uh->GetUnit(unitID) == NULL) {
		return 0;
	}

	Command cmd;

	// commandTbl = {
	//     "type" = CMD_*,
	//     "opts" = *_KEY,
	//     "args" = {
	//         [0] = 123,
	//         [1] = 456,
	//         [2] = 789,
	//         ...
	//     },
	//     [ "tag" = 31415927182818, ]
	//     [ "ttl" = MAX_INT, ]
	// }

	lua_pushnil(L);

	while (lua_next(L, tableIdx) != 0) {
		const int keyType = lua_type(L, -2);
		const int valType = lua_type(L, -1);

		if (keyType == LUA_TSTRING) {
			const std::string key = lua_tostring(L, -2);

			switch (valType) {
				case LUA_TNUMBER: {
					     if (key == "type") { cmd.id      = lua_tonumber(L, -1); }
					else if (key == "opts") { cmd.options = lua_tonumber(L, -1); }
					else if (key == "tag" ) { cmd.tag     = lua_tonumber(L, -1); }
					else if (key == "ttl" ) { cmd.timeOut = lua_tonumber(L, -1); }
				} break;
				case LUA_TTABLE: {
					// args sub-table (array)
					lua_pushnil(L);

					while (lua_next(L, -2) != 0) {
						MAI_ASSERT(lua_type(L, -2) == LUA_TNUMBER); // idx (key)
						MAI_ASSERT(lua_type(L, -1) == LUA_TNUMBER); // arg (val)

						cmd.AddParam(lua_tonumber(L, -1));
						lua_pop(L, 1);
					}
				} break;
			}
		}

		// pop the VALUE
		lua_pop(L, 1);
	}

	lua_pushnumber(L, rcb->GiveOrder(unitID, &cmd));
	return 1;
}
