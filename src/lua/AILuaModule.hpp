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

	void SetGroup(AIGroup* g) { group = g; }
	int  GetMaxGroupSize() const { return maxGroupSize; }

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
};

#endif
