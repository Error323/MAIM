#ifndef AI_LUA_MODULE_LOADER_HDR
#define AI_LUA_MODULE_LOADER_HDR

#include <map>
#include <string>

struct lua_State;

class AIUnitDef;
class LuaModule;
class LuaModuleLoader {
public:
	LuaModuleLoader();
	~LuaModuleLoader();

	LuaModule* GetModule(const AIUnitDef*, unsigned int);
	lua_State* LoadLuaModule(const std::string&);

private:
	// each AI instance maintains a cache of Lua
	// VM states; there is exactly one state for
	// each unique type of Lua module (per AI);
	// there are never more states than unique
	// module script types
	std::map<std::string, lua_State*> luaStates;
};

#endif
