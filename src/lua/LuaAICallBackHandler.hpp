#ifndef LUA_AI_CALLBACK_HANDLER_HDR
#define LUA_AI_CALLBACK_HANDLER_HDR

struct AIHelper;
struct lua_State;
class LuaAICallBackHandler {
public:
	static void SetHelper(AIHelper* h) {
		aih = h;
	}

	static int EcoStateIsStallingMetal(lua_State*);
	static int EcoStateIsStallingEnergy(lua_State*);
	static int GameMapGetAmountOfLand(lua_State*);
	static int GameMapGetAmountOfWater(lua_State*);

private:
	static AIHelper* aih;
};

#endif
