#ifndef HELPER_HDR
#define HELPER_HDR

class IGlobalAICallback;
class IAICallback;
class IAICheats;

class Logger;
class Timer;

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
		aiunitdefhandler(0),
		ecostate(0),
		gamemap(0){}

	int team;

	void Init(IGlobalAICallback*, int);
	void Release();

	IAICallback*      rcb;   // regular callback handler
	IAICheats*        ccb;   // cheat callback handler

	Logger*           logger;
	Timer*            timer;

	AIUnitDefHandler* aiunitdefhandler;
	EcoState*         ecostate;
	GameMap*          gamemap;
};

#endif
