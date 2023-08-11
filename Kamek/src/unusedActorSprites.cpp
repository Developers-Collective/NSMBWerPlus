#include <game.h>
#include <profile.h>

//this one might be bugged, idk
const char* MantaMgr2FileList [] = {"manjirou", NULL};

class dMantaMgr2_c : public dStageActor_c {
public:
	static dActor_c *build();
	int onCreate();
	int onExecute();
	int onDelete();

	u32 mantaId;
};


const SpriteData MantaMgr2SpriteData = { ProfileId::MantaMgr2, 0, 0 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0};
Profile MantaMgr2Profile(&dMantaMgr2_c::build, SpriteId::MantaMgr2, &MantaMgr2SpriteData, ProfileId::MantaMgr2, ProfileId::MantaMgr2, "MantaMgr2", MantaMgr2FileList);


dActor_c* dMantaMgr2_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(dMantaMgr2_c));
	return new(buffer) dMantaMgr2_c;
}


s32 dMantaMgr2_c::onCreate() {
	this->mantaId = dStageActor_c::create(ProfileId::MANTA_MGR2, settings, &(this->pos), 0, 0)->id;
	return true;
}

int dMantaMgr2_c::onExecute() {
	checkZoneBoundaries(0);
	return true;
}

int dMantaMgr2_c::onDelete() {
	dStageActor_c* mantaMgr2 = (dStageActor_c*)fBase_c::searchById(this->mantaId);
	if(mantaMgr2) mantaMgr2->Delete(0);
	return true;
}

const char* WaterliftFileList [] = {"lift_han_wood", NULL};

class dWaterliftSpawner : public dStageActor_c {
public:
	static dActor_c *build();
	int onCreate();
	int onExecute();
	int onDelete();

	u32 liftID;
};


const SpriteData WaterliftSpawnerSpriteData = { ProfileId::Waterlift, 0, 0, 0 , 0, 0x10, 0x10, 0, 0, 0, 0, 8};
Profile WaterliftSpawnerProfile(&dWaterliftSpawner::build, SpriteId::Waterlift, &WaterliftSpawnerSpriteData, ProfileId::Waterlift, ProfileId::Waterlift, "Waterlift", WaterliftFileList);


dActor_c* dWaterliftSpawner::build() {
	void* buffer = AllocFromGameHeap1(sizeof(dWaterliftSpawner));
	return new(buffer) dWaterliftSpawner;
}


int dWaterliftSpawner::onCreate() {
	this->liftID = dStageActor_c::create(ProfileId::WATER_LIFT, settings, &(this->pos), 0, 0)->id;
	return true;
}

int dWaterliftSpawner::onExecute() {
	checkZoneBoundaries(0);
	return true;
}

int dWaterliftSpawner::onDelete() {
	dStageActor_c* lift = (dStageActor_c*)fBase_c::searchById(this->liftID);
	if(lift) lift->Delete(0);
	return true;
}

