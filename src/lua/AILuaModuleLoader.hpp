#ifndef AI_LUA_MODULE_LOADER_HDR
#define AI_LUA_MODULE_LOADER_HDR

#include <map>
#include <vector>

struct lua_State;

class AIUnitDef;
class LuaModule;
class LuaModuleLoader {
public:
	LuaModuleLoader();
	~LuaModuleLoader();

	// called for newly-created unit groups
	LuaModule* GetModule(const AIUnitDef*, unsigned int);

private:
	lua_State* LoadLuaModule(const std::string&);

	struct LuaModuleClass {
		bool operator < (const LuaModuleClass& mc) const {
			return (typeMask < mc.typeMask && terrMask < mc.terrMask && weapMask < mc.weapMask);
		}
		bool operator == (const LuaModuleClass& mc) const {
			return (typeMask == mc.typeMask && terrMask == mc.terrMask && weapMask == mc.weapMask);
		}

		unsigned int typeMask;
		unsigned int terrMask;
		unsigned int weapMask;
		unsigned int roleMask;
	};

	// each AI instance maintains a cache of Lua
	// VM states; there is exactly one state for
	// each unique type of Lua script present in
	// AI_LUA_DIR/{def/, mod/}
	//
	// this means multiple groups of the same type
	// of unit will share the same state, which is
	// possibly not what we want... can we not just
	// load a lua_State* per group? but that would
	// be inefficient wrt. group creation
	//
	std::map<LuaModuleClass, std::vector<lua_State*> > luaModuleStates;
	std::map<LuaModuleClass, std::vector<LuaModule*> > luaModules;
};

#endif
