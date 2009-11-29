#include <cassert>

#include "./LuaAICallBackHandler.hpp"
#include "../main/AILua.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"

AIHelper* LuaAICallBackHandler::currHelper = NULL;
LuaModule* LuaAICallBackHandler::currModule = NULL;

/*
int LuaAICallBackHandler::Avg(lua_State* L) {
	// get number of arguments passed to us from Lua
	int n = lua_gettop(L);
	float sum = 0;

	for (int i = 1; i <= n; i++) {
		assert(lua_isnumber(L, i));
		sum += lua_tonumber(L, i);
	}

	lua_pushnumber(L, sum / n); // avg
	lua_pushnumber(L, sum);     // sum

	// return number of arguments to Lua
	return 2;
}
*/


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
