#ifndef AI_LUA_MODULE_HDR
#define AI_LUA_MODULE_HDR

#include <map>
#include "../main/AITypes.hpp"

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
	bool CanRun();
	bool Update();

	void AddUnit(Uint32 unitID);
	void DelUnit(Uint32 unitID);

	void SetPriority(Uint32 p) { priority = p; }
	void SetGroup(AIGroup* g) { moduleGroup = g; }

	std::string GetName();
	Uint32 GetMinGroupSize();
	Uint32 GetMaxGroupSize();
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

	DECLARE_STRUCT(LuaModuleClass)
	struct LuaModuleClass {
		LuaModuleClass(): typeMask(0), terrMask(0), weapMask(0), roleMask(0) {
		}

		bool operator < (rcLuaModuleClass mc) const;
		bool operator == (rcLuaModuleClass mc) const {
			return (typeMask == mc.typeMask && terrMask == mc.terrMask && weapMask == mc.weapMask && roleMask == mc.roleMask);
		}
		rLuaModuleClass operator = (rcLuaModuleClass mc) {
			typeMask = mc.typeMask;
			terrMask = mc.terrMask;
			weapMask = mc.weapMask;
			roleMask = mc.roleMask;
			return *this;
		}

		Uint32 typeMask;
		Uint32 terrMask;
		Uint32 weapMask;
		Uint32 roleMask;

		friend std::ostream& operator << (std::ostream&, rcLuaModuleClass);
	};

	void SetModuleClass(rcLuaModuleClass c) { moduleClass = c; }
	rcLuaModuleClass GetModuleClass() const { return moduleClass; }

	friend std::ostream& operator<<(std::ostream&, rcLuaModule);

private:
	bool isValid;

	lua_State* moduleState;
	AIGroup* moduleGroup;
	LuaModuleClass moduleClass;

	// LUAMODULE_PRIORITY_*
	Uint32 priority;
};

#endif
