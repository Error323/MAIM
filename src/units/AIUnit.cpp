#include "Sim/Units/CommandAI/Command.h"
#include "Sim/Units/CommandAI/CommandQueue.h"
#include "System/float3.h"
#include "LegacyCpp/IAICallback.h"

#include "./AIUnit.hpp"
#include "./AIUnitDef.hpp"
#include "./AIUnitDefHandler.hpp"
#include "../main/AIHelper.hpp"
#include "../utils/Debugger.hpp"

void AIUnit::Update() {
	MAI_ASSERT(unitDef != 0);

	UpdateState();
	UpdateCommand();
	UpdateWait();

	lifeTime += 1;
}

void AIUnit::Init(unsigned int _unitID, int unsigned _builderID) {
	currCmdID = 0;

	lifeTime = 0;
	idleTime = 0;

	waiting = false;

	pos = ZeroVector;
	vel = ZeroVector;
	dir = ZeroVector;
	spd = 0.0f;
	health = 0.0f;

	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	pcUnitDef def = rcb->GetUnitDef(_unitID);
	pcAIUnitDef defAI = aih->GetAIUnitDefHandler()->GetUnitDefByID(def->id);

	unitDef   = defAI;
	unitID    = _unitID;
	builderID = _builderID;

 	// set the UnitDestroyedSubject unit
	// this is required because units are reusable
	SetUnitDestroyedSubjectID(unitID);

	// Remove all the observers of this subject
	AUnitDestroyedSubject::RemoveObservers();
}

void AIUnit::UpdateState() {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	if (rcb->GetUnitPos(unitID) == pos) {
		idleTime += 1;
	} else {
		idleTime = 0;
	}

	vel = rcb->GetUnitVel(unitID);
	pos = rcb->GetUnitPos(unitID);
	dir = (vel != ZeroVector)? (vel / vel.Length()): ZeroVector;
	spd = (idleTime == 0)? (vel.Length() * GAME_SPEED): 0.0f;
	health = rcb->GetUnitHealth(unitID);
}

void AIUnit::UpdateCommand() {
	pcCCommandQueue cq = AIHelper::GetActiveInstance()->GetCallbackHandler()->GetCurrentUnitCommands(unitID);
	pcCommand       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

	currCmdID = (uc != NULL)? (uc->id): 0;
}

void AIUnit::UpdateWait() {
	if (currCmdID == 0) {
		if (idleTime >= (GAME_SPEED * 5)) {
			// this unit has been doing nothing for at least 5
			// seconds, force it to be re-registered as idle by
			// by triggering the engine to fire a UnitIdle event
			// (which trickles down to this->group)
			Stop();
		}
	}
}



// check if this unit's CQ is non-empty
bool AIUnit::HasCommand() const {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	pcCCommandQueue cq = rcb->GetCurrentUnitCommands(unitID);
	pcCommand       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

	return (uc != NULL);
}

bool AIUnit::CanGiveCommand(int cmdID) const {
	if (cmdID < 0) {
		// build order
		return (unitDef->HaveBuildOptionDefID(-cmdID));
	} else {
		switch (cmdID) {
			case CMD_MOVE:    { return unitDef->isMobile;             } break;
			case CMD_ATTACK:  { return unitDef->GetDef()->canAttack;  } break;
			case CMD_PATROL:  { return unitDef->GetDef()->canPatrol;  } break;
			case CMD_FIGHT:   { return unitDef->GetDef()->canFight;   } break; // and has weapons?
			case CMD_DGUN:    { return unitDef->GetDef()->canDGun;    } break; // and has dgun weapon?
			case CMD_GUARD:   { return unitDef->GetDef()->canGuard;   } break;
			case CMD_RECLAIM: { return unitDef->GetDef()->canReclaim; } break;
			case CMD_REPAIR:  { return unitDef->GetDef()->canRepair;  } break;
			case CMD_RESTORE: { return unitDef->GetDef()->canRestore; } break;
			case CMD_ONOFF:   { return unitDef->GetDef()->onoffable;  } break;
			case CMD_WAIT:    { return true;                          } break; // universal
			case CMD_STOP:    { return true;                          } break; // universal

			default: {
				return false;
			} break;
		}
	}
}

int AIUnit::GiveCommand(pCommand c) const {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	return (rcb->GiveOrder(unitID, c));
}

int AIUnit::TryGiveCommand(pCommand c) const {
	if (CanGiveCommand(c->id)) {
		return (GiveCommand(c));
	}

	return -100;
}

int AIUnit::GetCommandQueueSize() const {
	pAIHelper aih = AIHelper::GetActiveInstance();
	pIAICallback rcb = aih->GetCallbackHandler();

	return (rcb->GetCurrentUnitCommands(unitID)->size());
}



void AIUnit::Stop() {
	Command c;
		c.id = CMD_STOP;
	GiveCommand(&c);

	idleTime = 0;
}

void AIUnit::Wait(bool w) {
	Command c;
		c.id = CMD_WAIT;
	GiveCommand(&c);

	waiting = w;
}

void AIUnit::Move(rcFloat3 goal) {
	if (pos != ZeroVector)
	{
		Command c;
			c.id = CMD_MOVE;
			c.params.push_back(goal.x);
			c.params.push_back(goal.y);
			c.params.push_back(goal.z);
		TryGiveCommand(&c);
	}
}

void AIUnit::SetActiveState(cBool wantActive) {
	if (CanGiveCommand(CMD_ONOFF))
	{
		if (active != wantActive) {
			active = !active;

			Command c;
				c.id = CMD_ONOFF;
				c.params.push_back(wantActive);
			GiveCommand(&c);
		}
	}
}



std::ostream& operator<<(std::ostream &out, rcAIUnit unit) {
	out << "Unit{name:";
	out << unit.unitDef->GetName();
	out << ", unitID:";
	out << unit.unitID;
	out << ", builderID:";
	out << unit.builderID;
	out << ", lifeTime:";
	out << unit.lifeTime;
	out << ", idleTime:";
	out << unit.idleTime;
	out << "}";

	return out;
}
