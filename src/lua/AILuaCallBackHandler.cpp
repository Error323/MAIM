#include <cassert>

#include "./AILuaCallBackHandler.hpp"
#include "./AILuaHeaders.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

AIHelper* LuaCallBackHandler::currHelper = NULL;
LuaModule* LuaCallBackHandler::currModule = NULL;

int LuaCallBackHandler::EcoStateIsStallingMetal(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushboolean(L, currHelper->ecoState->IsStallingMetal());
	return 1;
}
int LuaCallBackHandler::EcoStateIsStallingEnergy(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushboolean(L, currHelper->ecoState->IsStallingEnergy());
	return 1;
}


int LuaCallBackHandler::GameMapGetAmountOfLand(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushnumber(L, currHelper->gameMap->GetAmountOfLand());
	return 1;
}

int LuaCallBackHandler::GameMapGetAmountOfWater(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushnumber(L, currHelper->gameMap->GetAmountOfWater());
	return 1;
}
