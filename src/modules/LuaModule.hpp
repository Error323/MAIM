#ifndef LUA_MODULE_HDR
#define LUA_MODULE_HDR

#include "./AModule.hpp"

struct lua_State;
class LuaModule: public AModule {
public:
	LuaModule();
	~LuaModule() {}

	void Init(AIHelper* h) { aih = h; }
	void Release();
	bool LoadState(const std::string&);

	std::string GetName();

	void Filter(std::map<int, AIUnit*>&) {}
	bool CanRun();
	bool Update();

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
};

#endif
