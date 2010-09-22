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

	void Filter(const std::map<int, AIUnit*>&) {}
	bool CanRun();
	bool Update();

	void SetGroup(AIGroup* g) { group = g; }
	void UnitDestroyed(int unitID) { units.erase(unitID); }

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
	// ?? why does the *module* need to keep track of this??
	std::map<int, AIUnit*> units;

	AIGroup* group;

	unsigned moduleTypeMasks;
	unsigned moduleTerrainMasks;
	unsigned moduleWeaponMasks;
	unsigned moduleMoveMasks;
};

#endif
