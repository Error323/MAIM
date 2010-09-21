#ifndef AI_LUA_MODULE_LOADER_HDR
#define AI_LUA_MODULE_LOADER_HDR

#include <map>
#include <string>

struct lua_State;
struct AIHelper;

class LuaModuleLoader {
public:
	LuaModuleLoader(AIHelper* h): aih(h) {}
	~LuaModuleLoader();

	lua_State* LoadLuaModule(const std::string&);

private:
	// each AI instance maintains a cache of Lua
	// VM states; there is exactly one state for
	// each unique type of Lua module (per AI);
	// there are never more states than unique
	// module script types
	std::map<std::string, lua_State*> luaStates;

	AIHelper* aih;
};

#endif
