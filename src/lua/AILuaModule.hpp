#ifndef AI_LUA_MODULE_HDR
#define AI_LUA_MODULE_HDR

#include <map>


struct lua_State;
class AIHelper;
class AIUnit;
class AIGroup;

class LuaModule {
public:
	LuaModule();
	~LuaModule() {}

	void Init() {}
	void Release();
	bool LoadState(const std::string&);

	std::string GetName();

	bool CanRun();
	bool Update();

	void SetPriority(unsigned int p) { priority = p; }
	void SetGroup(AIGroup* g) { group = g; }
	unsigned int GetMaxGroupSize() const { return maxGroupSize; }
	unsigned int GetPriority() const { return priority; }

	bool IsSuited(unsigned, unsigned, unsigned, unsigned);
	bool IsValid() const { return isValid; }
	bool HaveGetName() const { return haveGetName; }
	bool HaveCanRun() const { return haveCanRun; }
	bool HaveUpdate() const { return haveUpdate; }

	bool SetLuaState(lua_State*);
	lua_State* GetLuaState() const { return luaState; }

	// note: order matters
	enum {
		LUAMODULE_PRIORITY_EMERGENCY = 0,
		LUAMODULE_PRIORITY_REACTIVE  = 1,
		LUAMODULE_PRIORITY_PROACTIVE = 2,
		LUAMODULE_NUM_PRIORITIES     = 3,
	};

private:
	bool isValid;
	bool haveGetName;
	bool haveCanRun;
	bool haveUpdate;

	// each Lua module has its own VM state
	lua_State* luaState;

	std::string name;

	AIGroup* group;

	unsigned moduleTypeMasks;
	unsigned moduleTerrainMasks;
	unsigned moduleWeaponMasks;
	unsigned moduleMoveMasks;

	// Should be read from <module>.lua
	unsigned int maxGroupSize;

	unsigned int priority;
};

#endif
