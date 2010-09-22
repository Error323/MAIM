#ifndef AI_LUA_MODULE_HDR
#define AI_LUA_MODULE_HDR

#include <map>


struct lua_State;
class AIHelper;
class AIUnit;
class AIGroup;

// Order matters
enum modulePriority {
	EMERGENCY = 0,
	REACTIVE = 1,
	PROACTIVE = 2,
	MODULE_PRIORITY_COUNT = 3
};

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

	void SetGroup(AIGroup* g) { group = g; }
	int  GetMaxGroupSize() const { return maxGroupSize; }
	int  GetPriority() const { return priority; }

	bool IsSuited(unsigned, unsigned, unsigned, unsigned);
	bool IsValid() const { return isValid; }
	bool HaveGetName() const { return haveGetName; }
	bool HaveCanRun() const { return haveCanRun; }
	bool HaveUpdate() const { return haveUpdate; }

	lua_State* GetLuaState() const { return luaState; }

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
	int maxGroupSize;

	modulePriority priority;
};

#endif
