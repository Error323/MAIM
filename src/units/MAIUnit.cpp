#include <cassert>

#include "ExternalAI/IAICallback.h"

#include "Sim/Misc/GlobalConstants.h"
#include "Sim/MoveTypes/MoveInfo.h"
#include "Sim/Units/UnitDef.h"
#include "Sim/Units/CommandAI/Command.h"
#include "Sim/Units/CommandAI/CommandQueue.h"
#include "System/float3.h"

#include "./MAIUnit.hpp"

/// FIXME: temporary make-it-compile measure
struct AIHelper {
	IAICallback* rcb;
};

void AIUnit::SetActiveState(bool wantActive) {
	if (CanGiveCommand(CMD_ONOFF)) {
		if (active != wantActive) {
			active = !active;

			Command c;
				c.id = CMD_ONOFF;
				c.params.push_back(wantActive);
			GiveCommand(&c);
		}
	}
}

void AIUnit::Init() {
	currCmdID = 0;

	age = 0;
	limboTime = 0;

	waiting = false;

	pos = ZeroVector;
	vel = ZeroVector;
	dir = ZeroVector;
}

void AIUnit::Update() {
	assert(unitDef != 0);

	UpdatePosition();
	UpdateCommand();
	UpdateWait();

	age += 1;
}

void AIUnit::UpdatePosition() {
	// velocity is always one frame behind
	//
	// dir cannot be derived reliably in all
	// circumstances, so just set it to zero
	// if inferred velocity is zero
	if (aih->rcb->GetUnitPos(id) == pos) {
		limboTime += 1;
	} else {
		limboTime = 0;
	}

	vel = aih->rcb->GetUnitPos(id) - pos;
	pos = aih->rcb->GetUnitPos(id);
	dir = (vel != ZeroVector)? (vel / vel.Length()): ZeroVector;
	spd = (limboTime == 0)? (vel.Length() * GAME_SPEED): 0.0f;
}

void AIUnit::UpdateCommand() {
	const CCommandQueue* cq = aih->rcb->GetCurrentUnitCommands(id);
	const Command*       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

	currCmdID = (uc != NULL)? (uc->id): 0;
}

void AIUnit::UpdateWait() {
	if (currCmdID == 0) {
		if (limboTime >= (GAME_SPEED * 5)) {
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
	const CCommandQueue* cq = aih->rcb->GetCurrentUnitCommands(id);
	const Command*       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

	return (uc != NULL);
}

bool AIUnit::CanGiveCommand(int cmdID) const {
	if (cmdID < 0) {
		// build order
		/// FIXME: temporary make-it-compile measure, incorrect
		return (unitDef->buildOptions.find(-cmdID) != unitDef->buildOptions.end());
	} else {
		switch (cmdID) {
		///	case CMD_MOVE:    { return unitDef->isMobile;   } break;
			case CMD_ATTACK:  { return unitDef->canAttack;  } break;
			case CMD_PATROL:  { return unitDef->canPatrol;  } break;
			case CMD_FIGHT:   { return unitDef->canFight;   } break; // and has weapons?
			case CMD_DGUN:    { return unitDef->canDGun;    } break; // and has dgun weapon?
			case CMD_GUARD:   { return unitDef->canGuard;   } break;
			case CMD_RECLAIM: { return unitDef->canReclaim; } break;
			case CMD_REPAIR:  { return unitDef->canRepair;  } break;
			case CMD_RESTORE: { return unitDef->canRestore; } break;
			case CMD_ONOFF:   { return unitDef->onoffable;  } break;
			case CMD_WAIT:    { return true;                } break; // universal
			case CMD_STOP:    { return true;                } break; // universal

			default: {
				return false;
			} break;
		}
	}
}

int AIUnit::GiveCommand(Command* c) {
	return (aih->rcb->GiveOrder(id, c));
}

int AIUnit::TryGiveCommand(Command* c) {
	if (CanGiveCommand(c->id)) {
		return (GiveCommand(c));
	}

	return -100;
}

void AIUnit::Stop() { Command c; c.id = CMD_STOP; GiveCommand(&c); limboTime = 0; }
void AIUnit::Wait(bool w) { Command c; c.id = CMD_WAIT; GiveCommand(&c); waiting = w; }
void AIUnit::Move(const float3& goal) {
	if (pos != ZeroVector) {
		Command c;
			c.id = CMD_MOVE;
			c.params.push_back(goal.x);
			c.params.push_back(goal.y);
			c.params.push_back(goal.z);
		TryGiveCommand(&c);
	}
}




float AIUnit::GetPositionETA(const float3& p) {
	return 1e30f;
}