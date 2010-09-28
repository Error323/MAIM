#ifndef AI_LUA_CALLOUT_HANDLER_HDR
#define AI_LUA_CALLOUT_HANDLER_HDR

struct AIHelper;
class LuaModule;
struct lua_State;
class LuaCallOutHandler {
public:
	static void SetActiveModule(LuaModule* m) { activeModule = m; }
	static LuaModule* GetActiveModule() { return activeModule; }

	static void RegisterFunctions(lua_State*);

private:
	struct SimStateCallOuts {
		static void RegisterFunctions(lua_State*);

		static int GetInitSimFrame(lua_State*);
		static int GetCurrSimFrame(lua_State*);
		static int IsPaused(lua_State*);
	};
	struct UnitStateCallOuts {
		static void RegisterFunctions(lua_State*);

		static int GetUnitPos(lua_State*);
		static int GetUnitVel(lua_State*);
		static int GetUnitHealth(lua_State*);
	};
	struct EcoStateCallOuts {
		static void RegisterFunctions(lua_State*);

		static int IsStallingMetal(lua_State*);
		static int IsStallingEnergy(lua_State*);
	};
	struct GameMapCallOuts {
		static void RegisterFunctions(lua_State*);

		static int GetAmountOfLand(lua_State*);
		static int GetAmountOfWater(lua_State*);
	};
	struct CommandCallOuts {
		static void RegisterFunctions(lua_State*);

		static int GiveCommand(lua_State*);
	};

	static LuaModule* activeModule;
};

#endif
