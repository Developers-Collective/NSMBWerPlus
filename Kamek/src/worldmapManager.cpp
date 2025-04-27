#include <game.h>
#include <common.h>
#include <profile.h>

//map manager actor, do whatever you want with it

int spawnMapManager() {
    dActor_c* worldmapManager = (dActor_c*)fBase_c::searchByProfileId(ProfileId::WM_MANAGER);
	if(!worldmapManager) {
		dActor_c::create(WM_MANAGER, 0, 0, 0);
	}
	return 0;
}

class dWMManager_c : public dActor_c {
public:
	static dActor_c* build();
	static dWMManager_c *instance;

	int onCreate();
};

dWMManager_c *dWMManager_c::instance = 0;

const char* WMManagerNameList[] = {0};
Profile WMManagerProfile(&dWMManager_c::build, ProfileId::WM_MANAGER, NULL, ProfileId::WM_SINKSHIP, ProfileId::WM_MANAGER, "WM_MANAGER", WMManagerNameList);

dActor_c* dWMManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMManager_c));
	dWMManager_c *c = new(buffer) dWMManager_c;

	instance = c;
	return c;
}

int dWMManager_c::onCreate() {
	//grab save file
	SaveFile *file = GetSaveFile();
	SaveBlock *block = file->GetBlock(file->header.current_file);
	//check for the new bit
    if (!(block->bitfield & SAVE_BIT_NSMBWERPLUS)) {
		dStockItem_c *stockItem = (dStockItem_c*)searchByProfileId(ProfileId::STOCK_ITEM);
		//set new_powerups based off old ones
        for (int i = 0; i < 7; i++) {
			if (block->new_powerups_available[i] == 0) { //don't set if we've already got stuff in new_powerups
				block->new_powerups_available[i] = block->powerups_available[i];
				stockItem->newCounts[i] = block->new_powerups_available[i];
			}
        }
		//set our new bit
        block->bitfield |= SAVE_BIT_NSMBWERPLUS;
    }
	return true;
}