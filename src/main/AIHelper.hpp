#ifndef HELPER_HDR
#define HELPER_HDR

#include "./Types.hpp"

DECLARE_CLASS(IGlobalAICallback)
DECLARE_CLASS(IAICallback)
DECLARE_CLASS(IAICheats)

DECLARE_CLASS(Logger)
DECLARE_CLASS(Timer)

DECLARE_CLASS(LuaModuleLoader)
DECLARE_CLASS(AIUnitDefHandler)
DECLARE_CLASS(AIUnitHandler)
DECLARE_CLASS(EcoState)
DECLARE_CLASS(GameMap)

DECLARE_STRUCT(AIHelper)

struct AIHelper {
public:
	AIHelper():
		rcb(0),
		ccb(0),
		logger(0),
		timer(0),
		luaModuleLoader(0),
		aiUnitDefHandler(0),
		aiUnitHandler(0),
		ecoState(0),
		gameMap(0) {}

	int team;

	void Init(pIGlobalAICallback, int);
	void Release();

	static void SetActiveInstance(pAIHelper h) { activeInstance = h; }
	static pAIHelper GetActiveInstance() { return activeInstance; }

	pIAICallback      rcb;   // regular callback handler
	pIAICheats        ccb;   // cheat callback handler

	pLogger           logger;
	pTimer            timer;

	pLuaModuleLoader  luaModuleLoader;

	pAIUnitDefHandler aiUnitDefHandler;
	pAIUnitHandler    aiUnitHandler;
	pEcoState         ecoState;
	pGameMap          gameMap;

	static pAIHelper activeInstance;
};

#endif
