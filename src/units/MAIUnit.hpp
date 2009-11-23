#ifndef XAI_UNIT_HDR
#define XAI_UNIT_HDR

#include "System/float3.h"

struct Command;
struct UnitDef;
struct AIHelper;

class AIUnit {
public:
	AIUnit(int iid, AIHelper* h):
		id(iid),
		currCmdID(0),
		active(true),
		waiting(false),
		unitDef(0),
		aih(h) {
	}

	void Init();
	void Update();

	bool GetActiveState() const { return active; }
	void SetActiveState(bool);

	void SetUnitDef(const UnitDef* def) { unitDef = def; }
	const UnitDef* GetUnitDef() const { return unitDef; }

	const float3& GetPos() const { return pos; }
	const float3& GetVel() const { return vel; }
	const float3& GetDir() const { return dir; }

	int GetID() const { return id; }
	int GetCurrCmdID() const { return currCmdID; }
	unsigned int GetAge() const { return age; }

	bool HasCommand() const;
	bool CanGiveCommand(int) const;
	int GiveCommand(Command*);
	int TryGiveCommand(Command*);

	float GetPositionETA(const float3&);
	void Move(const float3&);
	void Stop();

private:
	void UpdatePosition();
	void UpdateCommand();
	void UpdateWait();

	void Wait(bool);

	const int id;
	int currCmdID;

	float3 pos;
	float3 vel;
	float3 dir;
	float spd; // length of vel

	bool active;
	bool waiting;

	unsigned int age;
	unsigned int limboTime;

	const UnitDef* unitDef;

	AIHelper* aih;
};

#endif
