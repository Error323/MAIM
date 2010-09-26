#ifndef AI_LUA_CALLBACK_HANDLER_HDR
#define AI_LUA_CALLBACK_HANDLER_HDR

struct AIHelper;
class LuaModule;
struct lua_State;
class LuaCallBackHandler {
public:
	static void SetActiveModule(LuaModule* m) { activeModule = m; }
	static LuaModule* GetActiveModule() { return activeModule; }

	struct EcoStateCallBacks {
		static int IsStallingMetal(lua_State*);
		static int IsStallingEnergy(lua_State*);
	};
	struct GameMapCallBacks {
		static int GetAmountOfLand(lua_State*);
		static int GetAmountOfWater(lua_State*);

	};
	struct CommandCallBacks {
		static int GiveCommand(lua_State*);
	};

private:
	static LuaModule* activeModule;
};

#endif
