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

	void Release();
	bool CanRun();
	bool Update();

	std::string GetName();
	void SetPriority(unsigned int p) { priority = p; }
	void SetGroup(AIGroup* g) { moduleGroup = g; }
	unsigned int GetMaxGroupSize() const { return maxGroupSize; }
	unsigned int GetPriority() const { return priority; }

	bool IsValid() const { return isValid; }
	bool HaveGetName() const { return haveGetName; }
	bool HaveCanRun() const { return haveCanRun; }
	bool HaveUpdate() const { return haveUpdate; }

	bool SetModuleState(lua_State*);
	lua_State* GetModuleState() const { return moduleState; }


	// note: order matters
	enum {
		LUAMODULE_PRIORITY_EMERGENCY = 0,
		LUAMODULE_PRIORITY_REACTIVE  = 1,
		LUAMODULE_PRIORITY_PROACTIVE = 2,
		LUAMODULE_NUM_PRIORITIES     = 3,
	};

	struct LuaModuleClass {
		bool operator < (const LuaModuleClass& mc) const {
			return (typeMask < mc.typeMask && terrMask < mc.terrMask && weapMask < mc.weapMask);
		}
		bool operator == (const LuaModuleClass& mc) const {
			return (typeMask == mc.typeMask && terrMask == mc.terrMask && weapMask == mc.weapMask);
		}
		LuaModuleClass& operator = (const LuaModuleClass& mc) {
			typeMask = mc.typeMask;
			terrMask = mc.terrMask;
			weapMask = mc.weapMask;
			roleMask = mc.roleMask;
			return *this;
		}

		unsigned int typeMask;
		unsigned int terrMask;
		unsigned int weapMask;
		unsigned int roleMask;
	};

	void SetModuleClass(const LuaModuleClass& c) { moduleClass = c; }
	const LuaModuleClass& GetModuleClass() const { return moduleClass; }

private:
	bool isValid;
	bool haveGetName;
	bool haveCanRun;
	bool haveUpdate;

	lua_State* moduleState;
	AIGroup* moduleGroup;
	LuaModuleClass moduleClass;

	// Should be read from <module>.lua
	unsigned int maxGroupSize;

	// LUAMODULE_PRIORITY_*
	unsigned int priority;
};

#endif
