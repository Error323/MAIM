#include "Sim/Units/CommandAI/Command.h"
#include "LegacyCpp/IAICallback.h"

#include "./AILuaCallBackHandler.hpp"
#include "./AILuaHeaders.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../units/AIUnitHandler.hpp"
#include "../utils/Debugger.hpp"

LuaModule* LuaCallBackHandler::activeModule = NULL;

int LuaCallBackHandler::EcoStateCallBacks::IsStallingMetal(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushboolean(L, AIHelper::GetActiveInstance()->GetEcoState()->IsStallingMetal());
	return 1;
}
int LuaCallBackHandler::EcoStateCallBacks::IsStallingEnergy(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushboolean(L, AIHelper::GetActiveInstance()->GetEcoState()->IsStallingEnergy());
	return 1;
}


int LuaCallBackHandler::GameMapCallBacks::GetAmountOfLand(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetGameMap()->GetAmountOfLand());
	return 1;
}

int LuaCallBackHandler::GameMapCallBacks::GetAmountOfWater(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushnumber(L, AIHelper::GetActiveInstance()->GetGameMap()->GetAmountOfWater());
	return 1;
}


int LuaCallBackHandler::CommandCallBacks::GiveCommand(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 2);

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	if (!lua_isnumber(L, 1)) { return 0; } // unitID
	if (!lua_istable(L, 2)) { return 0; } // commandTbl

	const int unitID = lua_tointeger(L, 1);
	const int tableIdx = 2; // == -1

	if (aih->GetAIUnitHandler()->GetUnit(unitID) == NULL) {
		return 0;
	}

	Command c;

	// commandTbl = {
	//     type = CMD_*,
	//     opts = *_KEY,
	//     args = {
	//         [0] = 123,
	//         [1] = 456,
	//         [2] = 789,
	//         ...
	//     },
	//     [ tag = 31415927182818, ]
	//     [ ttl = MAX_INT, ]
	// }

	lua_pushnil(L);

	while (lua_next(L, tableIdx) != 0) {
		const int keyType = lua_type(L, -2);
		const int valType = lua_type(L, -1);

		if (keyType == LUA_TSTRING) {
			const std::string key = lua_tostring(L, -2);

			switch (valType) {
				case LUA_TNUMBER: {
					     if (key == "type") { c.id      = lua_tonumber(L, -1); }
					else if (key == "opts") { c.options = lua_tonumber(L, -1); }
					else if (key == "tag" ) { c.tag     = lua_tonumber(L, -1); }
					else if (key == "ttl" ) { c.timeOut = lua_tonumber(L, -1); }
				} break;
				case LUA_TTABLE: {
					// args
				} break;
			}
		}

		// pop the VALUE
		lua_pop(L, 1);
	}

	lua_pushnumber(L, rcb->GiveOrder(unitID, &c));
	return 1;
}
