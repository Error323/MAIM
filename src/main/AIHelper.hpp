#ifndef HELPER_HDR
#define HELPER_HDR

class IGlobalAICallback;
class IAICallback;
class IAICheats;

class Logger;
class Timer;

class LuaModuleLoader;
class AIUnitDefHandler;
class EcoState;
class GameMap;

struct AIHelper {
public:
	AIHelper():
		rcb(0),
		ccb(0),
		logger(0),
		timer(0),
		luaModuleLoader(0),
		aiUnitDefHandler(0),
		ecoState(0),
		gameMap(0) {}

	int team;

	void Init(IGlobalAICallback*, int);
	void Release();

	IAICallback*      rcb;   // regular callback handler
	IAICheats*        ccb;   // cheat callback handler

	Logger*           logger;
	Timer*            timer;

	LuaModuleLoader*  luaModuleLoader;

	AIUnitDefHandler* aiUnitDefHandler;
	EcoState*         ecoState;
	GameMap*          gameMap;
};

#endif
