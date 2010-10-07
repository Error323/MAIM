#ifndef AI_HELPER_HDR
#define AI_HELPER_HDR

#include "./AITypes.hpp"

DECLARE_CLASS(IGlobalAICallback)
DECLARE_CLASS(IAICallback)
DECLARE_CLASS(IAICheats)

DECLARE_CLASS(AILogger)
DECLARE_CLASS(AITimer)

DECLARE_CLASS(LuaModuleLoader)
DECLARE_CLASS(AIUnitDefHandler)
DECLARE_CLASS(AIUnitHandler)
DECLARE_CLASS(EcoState)
DECLARE_CLASS(GameMap)
DECLARE_CLASS(AIGroupHandler)

DECLARE_STRUCT(AIHelper)

struct AIHelper {
public:
	AIHelper(): team(-1), initialized(false), initFrame(0), currFrame(0) {
		rcb = 0;
		ccb = 0;

		aiLogger = 0;
		aiTimer = 0;

		luaModuleLoader = 0;

		aiUnitDefHandler = 0;
		aiUnitHandler = 0;
		aiGroupHandler = 0;

		ecoState = 0;
		gameMap = 0;
	}

	void Init(pIGlobalAICallback, unsigned int);
	void Release();

	bool Initialized() const { return initialized; }
	void SetInitFrame(unsigned int f) { initFrame = f; }
	void SetCurrFrame(unsigned int f) { currFrame = f; }
	unsigned int GetInitFrame() const { return initFrame; }
	unsigned int GetCurrFrame() const { return currFrame; }

	static void SetActiveInstance(pAIHelper h) { activeInstance = h; }
	static pAIHelper GetActiveInstance() { return activeInstance; }

	pIAICallback GetCallbackHandler() { return rcb; }
	pIAICheats GetCCallbackHandler() { return ccb; }

	pAILogger GetLogger() { return aiLogger; }
	pAITimer GetTimer() { return aiTimer; }

	pLuaModuleLoader GetLuaModuleLoader() { return luaModuleLoader; }
	pAIUnitDefHandler GetAIUnitDefHandler() { return aiUnitDefHandler; }
	pAIUnitHandler GetAIUnitHandler() { return aiUnitHandler; }
	pAIGroupHandler GetAIGroupHandler() { return aiGroupHandler; }
	pEcoState GetEcoState() { return ecoState; }
	pGameMap GetGameMap() { return gameMap; }

private:
	unsigned int team;

	bool initialized;
	unsigned int initFrame;
	unsigned int currFrame;

	pIAICallback      rcb;   // regular callback handler
	pIAICheats        ccb;   // cheat callback handler

	pAILogger         aiLogger;
	pAITimer          aiTimer;

	pLuaModuleLoader  luaModuleLoader;

	pAIUnitDefHandler aiUnitDefHandler;
	pAIUnitHandler    aiUnitHandler;
	pAIGroupHandler   aiGroupHandler;
	pEcoState         ecoState;
	pGameMap          gameMap;

	static pAIHelper activeInstance;
};

#endif
