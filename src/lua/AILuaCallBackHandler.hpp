#ifndef AI_LUA_CALLBACK_HANDLER_HDR
#define AI_LUA_CALLBACK_HANDLER_HDR

struct AIHelper;
class LuaModule;
struct lua_State;
class LuaCallBackHandler {
public:
	static void SetHelper(AIHelper* h) { currHelper = h; }
	static void SetModule(LuaModule* m) { currModule = m; }

	static int EcoStateIsStallingMetal(lua_State*);
	static int EcoStateIsStallingEnergy(lua_State*);
	static int GameMapGetAmountOfLand(lua_State*);
	static int GameMapGetAmountOfWater(lua_State*);

private:
	static AIHelper* currHelper;
	static LuaModule* currModule;
};

#endif
