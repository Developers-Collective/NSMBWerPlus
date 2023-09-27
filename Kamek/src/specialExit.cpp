#include <game.h>
#include <profileid.h>
#include <stage.h>
#include "boss.h"
#include "msgbox.h"
#include <profile.h>

const char *SpecialExitFileList[] = {"RYOMRes5", 0};

class dSpecialExit_c : public dStageActor_c {
	public:
		static dActor_c *build();

		bool hasBeenActivated;
		bool hasExitedStage;
		int onExecute();
};

/*****************************************************************************/
// Glue Code
const SpriteData SpecialExitSpriteData = {ProfileId::SpecialExit, 10, 10, 0, 0, 0x200, 0x200, 0, 0, 200, 200, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SpecialExitProfile(&dSpecialExit_c::build, SpriteId::SpecialExit, &SpecialExitSpriteData, ProfileId::WM_KINOBALLOON, ProfileId::SpecialExit, "SpecialExit", SpecialExitFileList);

dActor_c *dSpecialExit_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSpecialExit_c));
	dSpecialExit_c *c = new(buffer) dSpecialExit_c;
	return c;
}


int dSpecialExit_c::onExecute() {
	if (dFlagMgr_c::instance->flags & spriteFlagMask) {
		if (!hasBeenActivated) {
//			OSReport("Activating Palace Dude\n");
			hasBeenActivated = true;

			int delay;
			if (!(settings & 0xF000000)) {
				delay = 270;
				StopBGMMusic();
				SaveBlock *save = GetSaveFile()->GetBlock(-1);
				GameMgrP->switchPalaceFlag |= (1 << (settings >> 28));
			} else {
				delay = 1020;
			}

			dMsgBoxManager_c::instance->showMessage(
				settings & 0xFFFFFF, // message ID
				false, // cannot cancel
				delay // delay
				);
		}
	}

	if (hasBeenActivated) {
		if (hasExitedStage)
			return true;
//		OSReport("Palace Dude is activated, %d\n", dMsgBoxManager_c::instance->visible);
		if (!dMsgBoxManager_c::instance->visible) {
//			OSReport("Exiting\n");
			u32 wmsettings = 0;
			if (settings & 0xF000000) {
				SaveGame(0, false);
				wmsettings = 0x80000000;
			}
			ExitStage(ProfileId::WORLD_MAP, wmsettings, BEAT_LEVEL, MARIO_WIPE);
			hasExitedStage = true;
		}
	}

	return true;

}


