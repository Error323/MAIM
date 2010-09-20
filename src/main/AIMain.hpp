#ifndef AIMAIN_HDR
#define AIMAIN_HDR

#include "./HAIInterface.hpp"

class AIHelper;
class AIMain: public IGlobalAI {
public:
	AIMain();
	~AIMain();

	void InitAI(IGlobalAICallback*, int);
	void ReleaseAI();

	void UnitCreated(int, int);
	void UnitFinished(int);
	void UnitDestroyed(int, int);
	void UnitIdle(int);
	void UnitDamaged(int, int, float, float3);
	void EnemyDamaged(int, int, float, float3);
	void UnitMoveFailed(int);

	void EnemyEnterLOS(int);
	void EnemyLeaveLOS(int);
	void EnemyEnterRadar(int);
	void EnemyLeaveRadar(int);
	void EnemyDestroyed(int, int);

	void GotChatMsg(const char*, int);
	int HandleEvent(int, const void*);

	void Update();

private:
	unsigned int aiInstance;
	static unsigned int aiInstances;
	AIHelper *aih;
};

#endif
