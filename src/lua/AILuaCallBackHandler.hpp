#ifndef AI_LUA_CALLBACK_HANDLER_HDR
#define AI_LUA_CALLBACK_HANDLER_HDR

struct AIHelper;
class LuaModule;
struct lua_State;
class LuaCallBackHandler {
public:
	static void SetActiveModule(LuaModule* m) { activeModule = m; }
	static LuaModule* GetActiveModule() { return activeModule; }

	static int EcoStateIsStallingMetal(lua_State*);
	static int EcoStateIsStallingEnergy(lua_State*);
	static int GameMapGetAmountOfLand(lua_State*);
	static int GameMapGetAmountOfWater(lua_State*);

private:
	static LuaModule* activeModule;
};

#endif
