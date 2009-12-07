#include <cassert>

#include "./LuaAICallBackHandler.hpp"
#include "../main/AILua.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

AIHelper* LuaAICallBackHandler::currHelper = NULL;
LuaModule* LuaAICallBackHandler::currModule = NULL;

int LuaAICallBackHandler::EcoStateIsStallingMetal(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushboolean(L, currHelper->ecoState->IsStallingMetal());
	return 1;
}
int LuaAICallBackHandler::EcoStateIsStallingEnergy(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushboolean(L, currHelper->ecoState->IsStallingEnergy());
	return 1;
}


int LuaAICallBackHandler::GameMapGetAmountOfLand(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushnumber(L, currHelper->gameMap->GetAmountOfLand());
	return 1;
}

int LuaAICallBackHandler::GameMapGetAmountOfWater(lua_State* L) {
	assert(lua_gettop(L) == 0);
	lua_pushnumber(L, currHelper->gameMap->GetAmountOfWater());
	return 1;
}
