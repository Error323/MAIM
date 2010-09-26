#include "./AILuaCallBackHandler.hpp"
#include "./AILuaHeaders.hpp"
#include "../main/AIHelper.hpp"
#include "../globals/EcoState.hpp"
#include "../globals/GameMap.hpp"
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
	MAI_ASSERT(lua_gettop(L) == 1);

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	if (!lua_istable(L, 1)) {
		return 0;
	}

	// command = {
	//     type = CMD_*,
	//     opts = *_KEY,
	//     args = {
	//         [0] = 123,
	//         [1] = 456,
	//         [2] = 789,
	//         ...
	//     },
	//     [ tag = 31415927182818, ]
	//     [ timeOut = MAX_INT, ]
	// }

	return 0;
}
