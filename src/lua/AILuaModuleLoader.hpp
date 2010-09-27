#ifndef AI_LUA_MODULE_LOADER_HDR
#define AI_LUA_MODULE_LOADER_HDR

#include <map>
#include <vector>

#include "../units/AIUnitDef.hpp"

struct lua_State;

class LuaModule;
class LuaModuleLoader {
public:
	LuaModuleLoader();
	~LuaModuleLoader();

	// called for newly-created unit groups
	LuaModule* GetModule(const AIUnitDef*, unsigned int);

private:
	lua_State* LoadLuaModule(const std::string&, std::string*);

	// each AI instance maintains a cache of Lua
	// VM states; there is exactly one state for
	// each unique type of Lua script present in
	// AI_LUA_DIR/{def/, mod/}
	//
	// multiple AIGroup*'s can NOT share one LuaModule*
	// multiple LuaModule*'s CAN share one lua_State*
	std::map<AIUnitDef::AIUnitDefClass, std::vector<lua_State*> > luaModuleStates;
	std::map<AIUnitDef::AIUnitDefClass, std::vector<LuaModule*> > luaModules;
};

#endif
