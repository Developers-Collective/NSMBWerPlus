#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <profileid.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"
#include <profile.h>
extern "C" void dAcPy_vf3F8(void* player, dEn_c* monster, int t);

class daBalboa_c : public daBoss {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::mdl_c spikesModel;

	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c animationChr;

	int timer;
	int damage;
	float Baseline;
	float dying;
	Vec PopUp [3];
	char throwCount;
	char throwMax;
	float throwRate;
	char upsideDown;
	int isBigBoss;
	char isRevenging;
	int spinner;

	ActivePhysics spikeCollision;
	float spikeOffset;
	bool spikeGoingUp, spikeGoingDown;

	public: static dActor_c *build();

	void setupModels();
	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	bool prePlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);

	void addScoreWhenHit(void *other);

	USING_STATES(daBalboa_c);
	DECLARE_STATE(Grow);
	DECLARE_STATE(ManholeUp);
	DECLARE_STATE(HeadPoke);
	DECLARE_STATE(AllOut);
	DECLARE_STATE(ThrowWrench);
	DECLARE_STATE(BackDown);
	DECLARE_STATE(Outro);
	DECLARE_STATE(Damage);
	DECLARE_STATE(RevengeUp);
	DECLARE_STATE(Revenge);
};

const char* BalboaWrenchNameList[] = {"choropoo","lift_zen", NULL};
const SpriteData BalboaWrenchSpriteData = {ProfileId::BossBalboaWrench, 0, 0xFFFFFFF8, 0, 0xFFFFFFB0, 0xA0, 0x50, 0, 0, 0, 0, 8};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile BalboaWrenchProfile(&daBalboa_c::build, SpriteId::BossBalboaWrench, &BalboaWrenchSpriteData, ProfileId::AC_LIFT_ICE_SPRING, ProfileId::BossBalboaWrench, "BossBalboaWrench", BalboaWrenchNameList);

dActor_c *daBalboa_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBalboa_c));
	return new(buffer) daBalboa_c;
}

// Externs

	CREATE_STATE(daBalboa_c, Grow);
	CREATE_STATE(daBalboa_c, ManholeUp);
	CREATE_STATE(daBalboa_c, HeadPoke);
	CREATE_STATE(daBalboa_c, AllOut);
	CREATE_STATE(daBalboa_c, ThrowWrench);
	CREATE_STATE(daBalboa_c, BackDown);
	CREATE_STATE(daBalboa_c, Outro);
	CREATE_STATE(daBalboa_c, Damage);
	CREATE_STATE(daBalboa_c, RevengeUp);
	CREATE_STATE(daBalboa_c, Revenge);

// Collisions
	void balbieCollisionCallback(ActivePhysics *apThis, ActivePhysics *apOther);

	void balbieCollisionCallback(ActivePhysics *apThis, ActivePhysics *apOther) {
		if (apOther->owner->profileId != ProfileId::BossWrenchThrow) {
			dEn_c::collisionCallback(apThis, apOther);
		}
	}

	void daBalboa_c::addScoreWhenHit(void *other) {}

bool daBalboa_c::prePlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (apOther->owner->stageActorType == 1) {
		if (apOther->info.category2 == 7) {
			if (collisionCat7_GroundPound(apThis, apOther))
				return true;
		}
	}

	return dEn_c::prePlayerCollision(apThis, apOther);
}

	void daBalboa_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {

		char ret = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

		if(ret == 0) {
			this->dEn_c::playerCollision(apThis, apOther);
			this->_vf220(apOther->owner);
		}

		//FIXME hack to make multiple playerCollisions work
		deathInfo.isDead = 0;
		this->flags_4FC |= (1<<(31-7));
		this->counter_504[apOther->owner->which_player] = 0;
	}

	bool daBalboa_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {

		dActor_c *block = apOther->owner;
		dEn_c *mario = (dEn_c*)block;

		SpawnEffect("Wm_en_vshit", 0, &mario->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

		mario->speed.y = -mario->speed.y;
		mario->pos.y += mario->speed.y;

		if (mario->direction == 0) { mario->speed.x = 4.0; }
		else					  { mario->speed.x = -4.0; }

		mario->doSpriteMovement();
		mario->doSpriteMovement();

		if (isRevenging) {
			_vf220(apOther->owner);
		} else {
			this->damage -= 1;

			apOther->someFlagByte |= 2;

			PlaySoundAsync(this, SE_EMY_PENGUIN_DAMAGE);

			doStateChange(&StateID_Damage);
		}

		return true;
	}
	bool daBalboa_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
		return collisionCat7_GroundPound(apThis, apOther);
	}

	bool daBalboa_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}
	bool daBalboa_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}
	bool daBalboa_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}
	bool daBalboa_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}
	bool daBalboa_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
		dAcPy_vf3F8(apOther->owner, this, 3);
		return true;
	}


void daBalboa_c::setupModels() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResMdl mdl;
	nw4r::g3d::ResAnmChr anmChr;

	this->resFile.data = getResource("choropoo", "g3d/choropoo.brres");

	mdl = this->resFile.GetResMdl("choropoo");
	this->bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&this->bodyModel, 0);

	anmChr = this->resFile.GetResAnmChr("throw_1"); // 11
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	nw4r::g3d::ResFile togeRes;
	togeRes.data = getResource("lift_zen", "g3d/t01.brres");
	mdl = togeRes.GetResMdl("lift_togeU");
	spikesModel.setup(mdl, &allocator, 0, 1, 0);

	// throw_1 // 11
	// throw_2 // 75
	// throw_3 // 33
	// throw_4_left_hand // 87
	// throw_4_right_hand // 87
	// throw_5 // 23

	allocator.unlink();
}

// Animation Order...
// AppearLittle - Throw One, sound 0x21F
// Search - Throw two
// AppearFull - Throw 3 and sound 0x220
// Attack - Throw 4
// Disappear - Throw 5


void daBalboa_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daBalboa_c::onCreate() {

	setupModels();

	this->scale = (Vec){1.0, 1.0, 1.0};
	this->isBigBoss = (this->settings >> 28);

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 27.0;

	HitMeBaby.xDistToEdge = 18.0;
	HitMeBaby.yDistToEdge = 24.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFBAFFFE;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &balbieCollisionCallback;


	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	ActivePhysics::Info spikeInfo = {
		0.0f, 0.0f, 16.0f, 16.0f,
		3, 0, 0x4F, 0xFFBAFFFE, 0, &dEn_c::collisionCallback};
	spikeCollision.initWithStruct(this, &spikeInfo);
	spikeCollision.trpValue0 = 0.0f;
	spikeCollision.trpValue1 = 0.0f;
	spikeCollision.trpValue2 = -16.0f;
	spikeCollision.trpValue3 = 16.0f;
	spikeCollision.collisionCheckType = 3;

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xE000; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->upsideDown = 0;
	this->direction = 0; // Heading left.
	this->pos.z = -800.0;
	this->pos.y -= 8.0;
	this->damage = 3;
	this->isRevenging = 0;

	this->PopUp[0] = (Vec){this->pos.x, this->pos.y - 54.0, this->pos.z};
	this->PopUp[1] = (Vec){this->pos.x - 224.0, this->pos.y - 54.0, this->pos.z};
	this->PopUp[2] = (Vec){this->pos.x - 112.0, this->pos.y - 22.0, this->pos.z};
	this->PopUp[3] = (Vec){this->pos.x - 112.0, this->pos.y - 22.0, this->pos.z};


	doStateChange(&StateID_Grow);

	this->onExecute();
	return true;
}

int daBalboa_c::onDelete() {
	return true;
}

int daBalboa_c::onExecute() {
	acState.execute();
	updateModelMatrices();

	if (spikeGoingUp) {
		spikeOffset += 2.5f;
		if (spikeOffset >= 48.0f) {
			spikeOffset = 48.0f;
			spikeGoingUp = false;
		}
	} else if (spikeGoingDown) {
		spikeOffset -= 2.5f;
		if (spikeOffset <= 0.0f) {
			spikeOffset = 0.0f;
			spikeGoingDown = false;
		}
	}
	spikeCollision.info.yDistToCenter = 16.0f + spikeOffset;

	bodyModel._vf1C();

	return true;
}

int daBalboa_c::onDraw() {

	bodyModel.scheduleForDrawing();
	if (spikeOffset > 0.0f)
		spikesModel.scheduleForDrawing();

	return true;
}

void daBalboa_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	mMtx spikeMatrix;
	VEC3 spikeScale = {2.0f,1.8f,2.0f};
	spikeMatrix.translation(pos.x, pos.y + spikeOffset, pos.z);
	spikesModel.setDrawMatrix(spikeMatrix);
	spikesModel.setScale(&spikeScale);
	spikesModel.calcWorld(false);
}

// Grow State

	void daBalboa_c::beginState_Grow() {
		this->timer = 0;

		SetupKameck(this, Kameck);

		bindAnimChr_and_setUpdateRate("begin_boss", 1, 0.0, 0.6);
	}

	void daBalboa_c::executeState_Grow() {

		if(this->animationChr.isAnimationDone())
			this->animationChr.setCurrentFrame(0.0);

		this->timer += 1;

		bool ret;
		ret = GrowBoss(this, Kameck, 1.0, 2.25, 0, this->timer);

		if (ret) {
			PlaySound(this, SE_EMY_CHOROPU_BOUND);
			doStateChange(&StateID_BackDown);
		}
	}
	void daBalboa_c::endState_Grow() {
		CleanupKameck(this, Kameck);
	}


// ManholeUp State

	void daBalboa_c::beginState_ManholeUp() {

		bindAnimChr_and_setUpdateRate("throw_1", 1, 0.0, 1.0);

		this->timer = 0;

		int randChoice;
		randChoice = GenerateRandomNumber(3);

		this->pos = this->PopUp[randChoice];



		if 		(randChoice == 0) { // On the left side!
			this->rot.y = 0xE000;
			this->rot.z = 0;
			this->upsideDown = 0;
			this->direction = 0; }

		else if (randChoice == 1) {	// On the right side!
			this->rot.y = 0x2000;
			this->rot.z = 0;
			this->upsideDown = 0;
			this->direction = 1; }

		// else if (randChoice == 2) {	// On the right ceiling!
		// 	this->rot.y = 0xE000;
		// 	this->rot.z = 0x8000;
		// 	this->upsideDown = 1;
		// 	this->direction = 0; }

		// else if (randChoice == 3) {	// On the left ceiling!
		// 	this->rot.y = 0x2000;
		// 	this->rot.z = 0x8000;
		// 	this->upsideDown = 1;
		// 	this->direction = 1; }

		else if (randChoice == 2) {	// In the Center!
			char Pdir = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, this->pos);

			if (Pdir == 1) {
				this->rot.y = 0xE000;
				this->direction = 0; }
			else {
				this->rot.y = 0x2000;
				this->direction = 1; }
		}

		PlaySound(this, 0x21F);
	}

	void daBalboa_c::executeState_ManholeUp() {

		if (this->timer > 51) {
			doStateChange(&StateID_HeadPoke);
		}
		if (this->timer > 11) { }
		else {
			this->pos.y += 0.8182; // Height is 54 pixels, move up 9 pixels.
		}

		this->timer += 1;
	}
	void daBalboa_c::endState_ManholeUp() { }


// HeadPoke State

	void daBalboa_c::beginState_HeadPoke() {

		bindAnimChr_and_setUpdateRate("throw_2", 1, 0.0, 1.0);

		this->timer = 0;
	}

	void daBalboa_c::executeState_HeadPoke() {

		this->pos.y += 0.24; // Height is 54 pixels, move up 18 pixels.

		if(this->animationChr.isAnimationDone()) {
			doStateChange(&StateID_AllOut); }

	}
	void daBalboa_c::endState_HeadPoke() { }


// AllOut State

	void daBalboa_c::beginState_AllOut() {

		bindAnimChr_and_setUpdateRate("throw_3", 1, 0.0, 1.0);

		this->timer = 0;

		PlaySound(this, 0x220);
	}

	void daBalboa_c::executeState_AllOut() {


		this->pos.y += 0.8182; // Height is 54 pixels, move up 27 pixels.

		if(this->animationChr.isAnimationDone()) {
			doStateChange(&StateID_ThrowWrench);
		}
	}
	void daBalboa_c::endState_AllOut() { }


// ThrowWrench State

	void daBalboa_c::beginState_ThrowWrench() {

		this->throwCount = 0;
		if (this->isBigBoss == 1) {
			throwMax = 6;
			throwRate = 3.0;
		}
		else {
			throwMax = 4;
			throwRate = 2.0;
		}
		bindAnimChr_and_setUpdateRate("throw_4_right_hand", 1, 0.0, throwRate);
	}

	void daBalboa_c::executeState_ThrowWrench() {

		float frame = this->animationChr.getCurrentFrame();
		if (frame == 54.0) {
			u32 settings;
			u8 up = this->upsideDown;
			u8 throwc = this->throwCount;
			u8 dir;

			if (this->direction) { dir = 0; }
			else 				 { dir = 1; }

			settings = (dir) | (up << 1);
			settings = settings | (throwc & 1 << 8);

			if (this->isBigBoss == 1) { settings = settings | 0x10; }

			CreateActor(ProfileId::BossWrenchThrow, settings, this->pos, 0, 0);
		}

		if(this->animationChr.isAnimationDone()) {
			this->throwCount += 1;

			if (this->throwCount & 1) {
				bindAnimChr_and_setUpdateRate("throw_4_left_hand", 1, 0.0, throwRate);
			}
			else {
				bindAnimChr_and_setUpdateRate("throw_4_right_hand", 1, 0.0, throwRate);
			}
		}

		if (this->throwCount > throwMax) {
			doStateChange(&StateID_BackDown); }
	}
	void daBalboa_c::endState_ThrowWrench() { }


// BackDown State

	void daBalboa_c::beginState_BackDown() {

		bindAnimChr_and_setUpdateRate("throw_5", 1, 0.0, 1.0);

		this->timer = 0;

		PlaySound(this, 0x221);

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){2.0, 1.0, 1.0});
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){2.0, 1.0, 1.0});
	}

	void daBalboa_c::executeState_BackDown() {

		if (this->timer < 60) {
			this->pos.y -= 2.0;  // Height is 54 pixels, move down
		}

		if (this->timer > 90) {
			doStateChange(&StateID_ManholeUp); }

		this->timer += 1;

	}
	void daBalboa_c::endState_BackDown() { }


// Outro

	void daBalboa_c::beginState_Outro() {

		bindAnimChr_and_setUpdateRate("dead", 1, 0.0, 1.0);

		OutroSetup(this);
		this->timer = 0;
		this->rot.x = 0x0; // X is vertical axis
		this->rot.z = 0x0; // Z is ... an axis >.>

	}
	void daBalboa_c::executeState_Outro() {

		if(this->animationChr.isAnimationDone())
			this->animationChr.setCurrentFrame(0.0);

		if (this->dying == 1) {
			if (this->timer > 180) { ExitStage(ProfileId::WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE); }
			if (this->timer == 60) { PlayerVictoryCries(this); }

			this->timer += 1;
			return;
		}

		bool ret;
		ret = ShrinkBoss(this, &this->pos, 2.25, this->timer);
		this->pos.y -= 0.02;

		if (ret == true) 	{
			BossExplode(this, &this->pos);
			this->dying = 1;
			this->timer = 0;
		}
		else 		{ PlaySound(this, SE_EMY_CHOROPU_SIGN); }

		this->timer += 1;

	}
	void daBalboa_c::endState_Outro() { }


// Damage

	void daBalboa_c::beginState_Damage() {

		bindAnimChr_and_setUpdateRate("dead", 1, 0.0, 0.5);

		this->timer = 0;
		this->removeMyActivePhysics();
	}
	void daBalboa_c::executeState_Damage() {

		if (this->timer > 6) { doStateChange(&StateID_RevengeUp); }

		if(this->animationChr.isAnimationDone()) {
			this->animationChr.setCurrentFrame(0.0);

			this->timer += 1;
		}

		if (this->timer > 3) {
			this->pos.y -= 5.0; // Height is 54 pixels, move down
		}
		else if (this->timer > 2) {
			if (this->damage == 0) {
				StopBGMMusic();
				doStateChange(&StateID_Outro);
			}
			this->pos.y -= 3.5; // Height is 54 pixels, move down
		}
		else if (this->timer > 1) {
			this->pos.y -= 1.0; // Height is 54 pixels, move down
		}
		else if (this->timer > 0) {
			this->pos.y += 1.0; // Height is 54 pixels, move down
		}
		else {
			this->pos.y += 3.5; // Height is 54 pixels, move down
		}

	}
	void daBalboa_c::endState_Damage() {
		this->addMyActivePhysics();
	}


// Revenge Up

	void daBalboa_c::beginState_RevengeUp() {

		this->pos = this->PopUp[2];
		this->rot.y = 0;

		isRevenging = 1;
		bindAnimChr_and_setUpdateRate("throw_3", 1, 0.0, 1.0);
		spikeGoingUp = true;
		spikeCollision.addToList();

		PlaySound(this, 0x220);
	}
	void daBalboa_c::executeState_RevengeUp() {

		this->pos.y += 1.6363; // Height is 54 pixels, move up 27 pixels.

		if(this->animationChr.isAnimationDone()) {
			doStateChange(&StateID_Revenge);
		}

	}
	void daBalboa_c::endState_RevengeUp() { }


// Revenge

	void daBalboa_c::beginState_Revenge() {
		spinner = 0;

		this->throwCount = 0;
		if (this->isBigBoss == 1) {
			throwMax = 16;
			throwRate = 5.0;
		}
		else {
			throwMax = 12;
			throwRate = 3.0;
		}
		bindAnimChr_and_setUpdateRate("throw_4_right_hand", 1, 0.0, throwRate);
	}
	void daBalboa_c::executeState_Revenge() {

		float frame = this->animationChr.getCurrentFrame();

		rot.y = 16384.0 * (frame / 87.0) + (16384.0 * spinner);

		if (frame == 60.0) {
			u32 settings;
			u8 up = this->upsideDown;
			u8 throwc = this->throwCount;
			u8 dir;

			if (spinner < 2)	 { dir = 0; }
			else 				 { dir = 1; }

			settings = (dir) | (up << 1);
			settings = settings | (throwc & 1 << 8);

			if (this->isBigBoss == 1) { settings = settings | 0x10; }

			CreateActor(ProfileId::BossWrenchThrow, settings, this->pos, 0, 0);
		}

		if(this->animationChr.isAnimationDone()) {
			this->throwCount += 1;
			spinner += 1;
			if (spinner == 4) { spinner = 0; }

			if (this->throwCount & 1) {
				bindAnimChr_and_setUpdateRate("throw_4_left_hand", 1, 0.0, throwRate);
			}
			else {
				bindAnimChr_and_setUpdateRate("throw_4_right_hand", 1, 0.0, throwRate);
			}
		}

		if (this->throwCount > throwMax) {
			doStateChange(&StateID_BackDown); }

	}
	void daBalboa_c::endState_Revenge() {
			isRevenging = 0;
			spikeGoingDown = true;
			spikeCollision.removeFromList();
	}

// Wrench Sprite

class daWrench : public dEn_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;

	int timer;
	char Kaboom;
	char direction;
	char front;
	float ymod;
	int lifespan;
	u32 cmgr_returnValue;

	public: static dActor_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);


	USING_STATES(daWrench);
	DECLARE_STATE(Straight);
	DECLARE_STATE(Kaboom);

};

CREATE_STATE(daWrench, Straight);
CREATE_STATE(daWrench, Kaboom);


extern "C" void *PlayWrenchSound(dEn_c *);


void daWrench::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { DamagePlayer(this, apThis, apOther); }

bool daWrench::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daWrench::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) { 
	return false; 
}
bool daWrench::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daWrench::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	SpawnEffect("Wm_ob_cmnboxgrain", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){0.5, 0.5, 0.5});

	PlaySoundAsync(this, SE_BOSS_JR_FLOOR_BREAK);
	this->Delete(1);
	return true;
}
bool daWrench::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daWrench::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) { 
	this->_vf220(apOther->owner);

	SpawnEffect("Wm_ob_cmnboxgrain", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){0.5, 0.5, 0.5});

	PlaySoundAsync(this, SE_BOSS_JR_FLOOR_BREAK);
	this->Delete(1);
	return true;
}

const char* WrenchThrowNameList[] = {"choropoo", NULL};
const SpriteData WrenchThrowSpriteData = {ProfileId::BossWrenchThrow, 8, 0xFFFFFFE0, 0, 0x10, 8, 0x18, 0, 0, 0, 0, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile WrenchThrowProfile(&daWrench::build, SpriteId::BossWrenchThrow, &WrenchThrowSpriteData, ProfileId::EN_GAKE_NOKO, ProfileId::BossWrenchThrow, "BossWrenchThrow", WrenchThrowNameList);

dActor_c *daWrench::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daWrench));
	return new(buffer) daWrench;
}


int daWrench::onCreate() {
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile rf(getResource("choropoo", "g3d/choropoo.brres"));
	bodyModel.setup(rf.GetResMdl("spanner"), &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();
	
	
	this->direction = this->settings & 0xF;
	this->Kaboom = (this->settings >> 4) & 0xF;
	this->front = (this->settings >> 8) & 0xF;
	
	ActivePhysics::Info HitMeBaby;
	
	if (this->Kaboom == 0) {
		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 0.0;
		HitMeBaby.xDistToEdge = 5.0;
		HitMeBaby.yDistToEdge = 5.0;
	
		this->scale.x = 1.25;
		this->scale.y = 1.25;
		this->scale.z = 1.25;
	}
	
	else {
		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 0.0;
		HitMeBaby.xDistToEdge = 8.0;
		HitMeBaby.yDistToEdge = 8.0;
	
		this->scale.x = 2.0;
		this->scale.y = 2.0;
		this->scale.z = 2.0;
	}
	
	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x47;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	spriteSomeRectX = 5.0f;
	spriteSomeRectY = 5.0f;
	_320 = 0.0f;
	_324 = 5.0f;

	// These structs tell stupid collider what to collide with - these are from koopa troopa
	static const lineSensor_s below(12<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();


	if (this->direction == 0) 	   { // Ground Facing Left
		this->pos.x -= 0.0; // -32 to +32
		this->pos.y += 36.0;
		this->rot.z = 0x2000;
	}
	else if (this->direction == 1) { // Ground Facing Right
		this->pos.x += 0.0; // +32 to -32
		this->pos.y += 36.0;
		this->rot.z = 0xE000;
	}
	if (this->front == 1) { this->pos.z = -1804.0; }
	else 				  { this->pos.z =  3300.0; }

	
	if (this->Kaboom) {
		doStateChange(&StateID_Kaboom); }
	else {
		doStateChange(&StateID_Straight); }
	
	this->onExecute();
	return true;
}


int daWrench::onDelete() {
	return true;
}

int daWrench::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daWrench::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


int daWrench::onExecute() {
	acState.execute();
	updateModelMatrices();

	float rect[] = {this->_320, this->_324, this->spriteSomeRectX, this->spriteSomeRectY};
	int ret = this->outOfZone(this->pos, (float*)&rect, this->currentZoneID);
	if(ret) {
		this->Delete(1);
	}

	return true;
}


void daWrench::beginState_Kaboom() { 
	float rand = (float)GenerateRandomNumber(10) * 0.4;

	if (this->direction == 0) { // directions 1 spins clockwise, fly rightwards
		speed.x = 1.0 + rand; 
	}
	else {						// directions 0 spins anti-clockwise, fly leftwards
		speed.x = -1.0 - rand; 
	}

	speed.y = 6.0;
}
void daWrench::executeState_Kaboom() { 

	speed.y = speed.y - 0.01875;

	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();

	cmgr_returnValue = collMgr.isOnTopOfTile();
	collMgr.calculateBelowCollisionWithSmokeEffect();

	if (collMgr.isOnTopOfTile()) {
		// hit the ground
		PlaySoundAsync(this, SE_BOSS_JR_BOMB_BURST);

		SpawnEffect("Wm_en_burst_s", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){0.75, 0.75, 0.75});
		SpawnEffect("Wm_mr_wirehit", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.25, 1.25, 1.25});
		this->Delete(1);
	}
	if (collMgr.outputMaybe & (0x15 << direction)) {
		// hit the wall
		PlaySoundAsync(this, SE_BOSS_JR_BOMB_BURST);
 
		if (this->direction == 0) { // directions 1 spins clockwise, fly rightwards
			SpawnEffect("Wm_en_burst_s", 0, &this->pos, &(S16Vec){0,0x4000,0}, &(Vec){0.75, 0.75, 0.75});
			SpawnEffect("Wm_mr_wirehit", 0, &this->pos, &(S16Vec){0,0x4000,0}, &(Vec){1.25, 1.25, 1.25});
		}
		else {						// directions 0 spins anti-clockwise, fly leftwards
			SpawnEffect("Wm_en_burst_s", 0, &this->pos, &(S16Vec){0,0xE000,0}, &(Vec){0.75, 0.75, 0.75});
			SpawnEffect("Wm_mr_wirehit", 0, &this->pos, &(S16Vec){0,0xE000,0}, &(Vec){1.25, 1.25, 1.25});
		}

		this->Delete(1);
	}

	if (this->direction == 1) { // directions 1 spins clockwise, fly rightwards
		this->rot.z -= 0x1000; }
	else {						// directions 0 spins anti-clockwise, fly leftwards
		this->rot.z += 0x1000; }	
	
	PlayWrenchSound(this);

}
void daWrench::endState_Kaboom() { }



void daWrench::beginState_Straight() { 
	float rand = (float)GenerateRandomNumber(10) * 0.4;

	if (this->direction == 0) { // directions 1 spins clockwise, fly rightwards
		speed.x = 1.0 + rand; 
	}
	else {						// directions 0 spins anti-clockwise, fly leftwards
		speed.x = -1.0 - rand; 
	}

	speed.y = 6.0;
}
void daWrench::executeState_Straight() { 

	speed.y = speed.y - 0.01875;

	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();

	cmgr_returnValue = collMgr.isOnTopOfTile();
	collMgr.calculateBelowCollisionWithSmokeEffect();

	if (collMgr.isOnTopOfTile()) {
		// hit the ground
		PlaySoundAsync(this, SE_BOSS_JR_FLOOR_BREAK);

		SpawnEffect("Wm_en_burst_s", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){0.75, 0.75, 0.75});
		this->Delete(1);
	}
	if (collMgr.outputMaybe & (0x15 << direction)) {
		// hit the wall
		PlaySoundAsync(this, SE_BOSS_JR_FLOOR_BREAK);

		SpawnEffect("Wm_en_burst_s", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){0.75, 0.75, 0.75});
		this->Delete(1);
	}

	if (this->direction == 1) { // directions 1 spins clockwise, fly rightwards
		this->rot.z -= 0x1000; }
	else {						// directions 0 spins anti-clockwise, fly leftwards
		this->rot.z += 0x1000; }	
	
	PlayWrenchSound(this);

}
void daWrench::endState_Straight() { }