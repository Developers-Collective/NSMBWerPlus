#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <profileid.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"
#include <profile.h>

extern "C" void *BowserExitDemoState(void *, unsigned int);
extern "C" void *ForceMarioExitDemoMode(void *, unsigned int);
extern "C" void *BowserFireballCollision(dEn_c *, ActivePhysics *, ActivePhysics *);
extern "C" void *BowserDamageAnmClr(dEn_c *);
extern "C" void *BowserDamageStepTwo(dEn_c *);
extern "C" void *BowserDamageNormal(dEn_c *);
extern "C" void *BowserDamageKill(dEn_c *);
extern "C" void *BowserDamageEnd(dEn_c *);

int BridgeBowserHP = 2;
int lastBomb = 0;

extern bool HackyBombDropVariable;

void BowserDoomSpriteCollision(dEn_c *bowser, ActivePhysics *apThis, ActivePhysics *apOther) {
	// If you collide with something or other, call the fireball collision

	if (apOther->owner->profileId == ProfileId::EN_BOSS_BOMB_DROPPED) {

		if (lastBomb == apOther->owner->id) { return; }
		if (!HackyBombDropVariable) return;
		HackyBombDropVariable = false;

		// void * bowserClass = (void*)(((u32)bowser) + 0x5F8);
		// int HP = *(int*)(((u32)bowserClass) + 4);

		OSReport("HP: %d", BridgeBowserHP);

		if (BridgeBowserHP <= 0) {
			BridgeBowserHP = 0;

			*(int*)(((u32)bowser) + 0x540) = 0x28;

			BowserDamageAnmClr(bowser);

			BowserDamageStepTwo(bowser);
			BowserDamageKill(bowser);

			// WeirdLevelEndClass->sub_8005CB60(*otherActor->returnPtrToField38D());

			// this->vf300(otherActor);
			BowserDamageEnd(bowser);

			// daBossKoopaDemo_c *BowserDemo = (daBossKoopaDemo_c*)fBase_c::searchByProfileId(ProfileId::BOSS_KOOPA_DEMO, 0);
			daBossKoopa_c *BowserClass = (daBossKoopa_c*)bowser;
			OSReport("Koopa Controller: %x", BowserClass);
			BowserClass->doStateChange(&daBossKoopa_c::StateID_Fall);
			dFlagMgr_c::instance->set(3, 0, true, false, false);

			BridgeBowserHP = 2;

		}
		else {
			*(int*)(((u32)bowser) + 0x540) = 0x28;

			BowserDamageAnmClr(bowser);
			BowserDamageNormal(bowser);

			BridgeBowserHP -= 1;
		}

		lastBomb = apOther->owner->id;

		dEn_c * bomb = (dEn_c*)apOther->owner;
		dFlagMgr_c::instance->set(bomb->settings & 0xFF, 0, true, false, false);
		bomb->kill();
	}

	return;
}

extern "C" void initializeMagic(dStageActor_c *); 
void BowserDoomStart(dStageActor_c *Controller) {
	if (Controller->settings & 1) {
		OSReport("Here we go!");

		dEn_c *Bowser = (dEn_c*)fBase_c::searchByProfileId(ProfileId::EN_BOSS_KOOPA, 0);
		Bowser->Delete(1);
		lastBomb = 0;
	} else {
		initializeMagic(Controller);
	}
}

extern "C" void executeMagic(dStageActor_c *); 
void BowserDoomExecute(dStageActor_c *Controller) {
	if (Controller->settings & 1) {
		dFlagMgr_c::instance->set(2, 0, true, false, false);
		Controller->Delete(1);
	} else {
		executeMagic(Controller);
	}
}

void BowserDoomEnd(dStageActor_c *Controller) {
	if (Controller->settings & 1) {
		OSReport("Bai bai everybody");
		Controller->Delete(1);
	}
}

void BowserStartEnd(dStageActor_c *Controller) {
	if (Controller->settings & 1) {
		dFlagMgr_c::instance->set(1, 0, true, false, false);
	}
}

u32 isBowserImmuneToFireBalls() {
	fBase_c *controller = fBase_c::searchByProfileId(ProfileId::BOSS_KOOPA_DEMO);
	if (controller != NULL && controller->settings & 1) {
		return 1;
	}
	return 0;
}

extern "C" void setDeathInfo_Quake_Boss(dEn_c *, int);
void BowserPowBlock(dEn_c *bowser, int isMPGP) {
	fBase_c *controller = fBase_c::searchByProfileId(ProfileId::BOSS_KOOPA_DEMO);
	if (controller != NULL && controller->settings & 1) {
		return;
	} else {
		setDeathInfo_Quake_Boss(bowser, isMPGP);
	}
} 