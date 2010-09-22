#include "./AILuaCallBackHandler.hpp"
#include "./AILuaHeaders.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
#include "../utils/Debugger.hpp"

LuaModule* LuaCallBackHandler::activeModule = NULL;

int LuaCallBackHandler::EcoStateIsStallingMetal(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushboolean(L, AIHelper::GetActiveInstance()->ecoState->IsStallingMetal());
	return 1;
}
int LuaCallBackHandler::EcoStateIsStallingEnergy(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushboolean(L, AIHelper::GetActiveInstance()->ecoState->IsStallingEnergy());
	return 1;
}


int LuaCallBackHandler::GameMapGetAmountOfLand(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushnumber(L, AIHelper::GetActiveInstance()->gameMap->GetAmountOfLand());
	return 1;
}

int LuaCallBackHandler::GameMapGetAmountOfWater(lua_State* L) {
	MAI_ASSERT(lua_gettop(L) == 0);
	lua_pushnumber(L, AIHelper::GetActiveInstance()->gameMap->GetAmountOfWater());
	return 1;
}
