#ifndef LUA_MODULE_LOADER_HDR
#define LUA_MODULE_LOADER_HDR

#include <map>
#include <string>

struct AIHelper;
class AModule;
class LuaModuleLoader {
public:
	LuaModuleLoader(AIHelper* h): aih(h) {}
	~LuaModuleLoader();

	AModule* LoadLuaModule(const std::string&);

private:
	std::map<std::string, AModule*> luaModules;

	AIHelper* aih;
};

#endif
