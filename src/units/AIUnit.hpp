#ifndef AI_UNIT_HDR
#define AI_UNIT_HDR

#include "System/float3.h"
#include "../main/Types.hpp"
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

	void Init();
	void Update();
	void Reset(int, int);

	cBool GetActiveState() const { return active; }
	void SetActiveState(cBool);

	void SetUnitDef(pcAIUnitDef def) { unitDef = def; }
	pcAIUnitDef GetUnitDef() const { return unitDef; }

	rcFloat3 GetPos() const { return pos; }
	rcFloat3 GetVel() const { return vel; }
	rcFloat3 GetDir() const { return dir; }

	cInt GetID() const { return unitID; }
	cInt GetCurrCmdID() const { return currCmdID; }
	cUint32 GetAge() const { return age; }

	cBool HasCommand() const;
	cBool CanGiveCommand(int) const;
	cInt GiveCommand(pCommand) const;
	cInt TryGiveCommand(pCommand) const;
	cInt GetCommandQueueSize() const;

	cFloat GetPositionETA(rcFloat3) const;
	void Move(rcFloat3);
	void Stop();
	void Wait(cBool);

private:
	void UpdatePosition();
	void UpdateCommand();
	void UpdateWait();

	int unitID;    // the unit's runtie id
	int builderID; // the unit's builder/creator
	int currCmdID;

	float3 pos;
	float3 vel;
	float3 dir;
	float spd; // length of vel

	bool active;
	bool waiting;

	Uint32 age;
	Uint32 limboTime;

	pcAIUnitDef unitDef;
};

#endif
