#include <cassert>

#include "../main/HAIInterface.hpp"
#include "../main/HEngine.hpp"

#include "./AIUnit.hpp"
#include "./AIUnitDef.hpp"
#include "../main/AIHelper.hpp"

void AIUnit::SetActiveState(cBool wantActive) {
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
	if (AIHelper::GetActiveInstance()->rcb->GetUnitPos(id) == pos) {
		limboTime += 1;
	} else {
		limboTime = 0;
	}

	vel = AIHelper::GetActiveInstance()->rcb->GetUnitPos(id) - pos;
	pos = AIHelper::GetActiveInstance()->rcb->GetUnitPos(id);
	dir = (vel != ZeroVector)? (vel / vel.Length()): ZeroVector;
	spd = (limboTime == 0)? (vel.Length() * GAME_SPEED): 0.0f;
}

void AIUnit::UpdateCommand() {
	pcCCommandQueue cq = AIHelper::GetActiveInstance()->rcb->GetCurrentUnitCommands(id);
	pcCommand       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

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
cBool AIUnit::HasCommand() const {
	pcCCommandQueue cq = AIHelper::GetActiveInstance()->rcb->GetCurrentUnitCommands(id);
	pcCommand       uc = (cq != NULL && !cq->empty())? &(cq->front()): NULL;

	return (uc != NULL);
}

cBool AIUnit::CanGiveCommand(int cmdID) const {
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

cInt AIUnit::GiveCommand(pCommand c) const {
	return (AIHelper::GetActiveInstance()->rcb->GiveOrder(id, c));
}

cInt AIUnit::TryGiveCommand(pCommand c) const {
	if (CanGiveCommand(c->id)) {
		return (GiveCommand(c));
	}

	return -100;
}

cInt AIUnit::GetCommandQueueSize() const {
	return AIHelper::GetActiveInstance()->rcb->GetCurrentUnitCommands(id)->size();
}

void AIUnit::Stop() { 
	Command c; 
	c.id = CMD_STOP; 
	GiveCommand(&c); 
	limboTime = 0; 
}

void AIUnit::Wait(cBool w) { 
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




cFloat AIUnit::GetPositionETA(rcFloat3 p) const {
	return 1e30f;
}
