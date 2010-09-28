#ifndef AI_UNIT_HDR
#define AI_UNIT_HDR

#include "System/float3.h"
#include "../main/AITypes.hpp"
#include "../observers/AUnitDestroyedSubject.hpp"

DECLARE_CLASS(Command)
DECLARE_CLASS(CCommandQueue)
DECLARE_CLASS(AIUnitDef)
DECLARE_CLASS(AIHelper)

DECLARE_CLASS(AIUnit)

class AIUnit: public AUnitDestroyedSubject {
public:
	AIUnit():
		builderID(-1),
		currCmdID(0),
		active(true),
		waiting(false),
		unitDef(0)
	{
	}

	void Init(unsigned int, unsigned int);
	void Update();

	cBool GetActiveState() const { return active; }
	void SetActiveState(cBool);

	void SetUnitDef(pcAIUnitDef def) { unitDef = def; }
	pcAIUnitDef GetUnitDef() const { return unitDef; }

	rcFloat3 GetPos() const { return pos; }
	rcFloat3 GetVel() const { return vel; }
	rcFloat3 GetDir() const { return dir; }

	float GetSpeed() const { return spd; }
	float GetHealth() const { return health; }

	Uint32 GetID() const { return unitID; }
	Uint32 GetBuilderID() const { return builderID; }
	Uint32 GetLifeTime() const { return lifeTime; }
	Uint32 GetIdleTime() const { return idleTime; }

	bool HasCommand() const;
	bool CanGiveCommand(int) const;
	int GiveCommand(pCommand) const;
	int TryGiveCommand(pCommand) const;
	int GetCommandQueueSize() const;
	int GetCurrCmdID() const { return currCmdID; }

	void Move(rcFloat3);
	void Stop();
	void Wait(bool);

	friend std::ostream& operator<<(std::ostream&, rcAIUnit);

private:
	void UpdateState();
	void UpdateCommand();
	void UpdateWait();

	Uint32 unitID;    // the unit's runtie id
	Uint32 builderID; // the unit's builder/creator

	int currCmdID;

	float3 pos;
	float3 vel;
	float3 dir;
	float spd;    // length of vel
	float health; // current amount of hit-points

	bool active;
	bool waiting;

	Uint32 lifeTime;
	Uint32 idleTime;

	pcAIUnitDef unitDef;
};

#endif
