#ifndef AI_LUA_MODULE_HDR
#define AI_LUA_MODULE_HDR

#include <map>
#include "../units/AIUnitDef.hpp"

DECLARE_STRUCT(lua_State)
DECLARE_CLASS(AIHelper)
DECLARE_CLASS(AIUnit)
DECLARE_CLASS(AIGroup)
DECLARE_CLASS(LuaModule)

class LuaModule {
public:
	LuaModule();
	~LuaModule() {}

	void Release();
	bool CanRun(Uint32 groupID);
	bool Update(Uint32 groupID);

	bool CanAddUnit(Uint32 groupID, Uint32 unitID);
	void AddUnit(Uint32 groupID, Uint32 unitID);
	void DelUnit(Uint32 groupID, Uint32 unitID);

	void SetPriority(Uint32 p) { priority = p; }
	void SetGroup(AIGroup* g) { moduleGroup = g; }

	std::string GetName();
	Uint32 GetMinGroupSize(Uint32 groupID);
	Uint32 GetMaxGroupSize(Uint32 groupID);
	Uint32 GetPriority() const { return priority; }

	bool IsValid() const { return isValid; }

	bool SetModuleState(lua_State*);
	lua_State* GetModuleState() const { return moduleState; }


	// note: order matters
	enum {
		LUAMODULE_PRIORITY_EMERGENCY = 0,
		LUAMODULE_PRIORITY_REACTIVE  = 1,
		LUAMODULE_PRIORITY_PROACTIVE = 2,
		LUAMODULE_NUM_PRIORITIES     = 3,
	};


	void SetUnitDefClass(const AIUnitDef::AIUnitDefClass& c) { unitDefClass = c; }
	const AIUnitDef::AIUnitDefClass& GetUnitDefClass() const { return unitDefClass; }

	friend std::ostream& operator<<(std::ostream&, rcLuaModule);

private:
	bool isValid;

	lua_State* moduleState;
	AIGroup* moduleGroup;
	AIUnitDef::AIUnitDefClass unitDefClass;

	// LUAMODULE_PRIORITY_*
	Uint32 priority;
};

#endif
