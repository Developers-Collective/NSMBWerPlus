#include <game.h>
#include <profile.h>

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
	dStageActor_c* lift = (dStageActor_c*)fBase_c::searchById(this->liftID);
	checkZoneBoundaries(0);
	this->pos = lift->pos;
	return true;
}

int dWaterliftSpawner::onDelete() {
	dStageActor_c* lift = (dStageActor_c*)fBase_c::searchById(this->liftID);
	if(lift) lift->Delete(0);
	return true;
}

