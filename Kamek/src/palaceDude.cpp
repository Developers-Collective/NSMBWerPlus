#include <game.h>
#include <stage.h>
#include "msgbox.h"
#include "boss.h" // for stopbgmmusic and crap
#include <profile.h>

const char *PalaceDudeFileList[] = {"RYOMRes5", 0};

class dPalaceDude_c : public dStageActor_c {
	public:
		static dActor_c *build();

		bool hasBeenActivated;
		bool hasExitedStage;
		int onExecute();
};

/*****************************************************************************/
// Glue Code
const SpriteData SpecialExitSpriteData = {ProfileId::AC_SWITCH_SET, 10, 10, 0, 0, 0x200, 0x200, 0, 0, 200, 200, 0};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile SpecialExitProfile(&dPalaceDude_c::build, SpriteId::AC_SWITCH_SET, &SpecialExitSpriteData, ProfileId::WM_KINOBALLOON, ProfileId::AC_SWITCH_SET, "AC_SWITCH_SET", PalaceDudeFileList);

dActor_c *dPalaceDude_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dPalaceDude_c));
	dPalaceDude_c *c = new(buffer) dPalaceDude_c;
	return c;
}

extern char CurrentWorld, CurrentLevel;
extern "C" u32 AssembleScWorldMapSettings(u32 world_num, int level_num, int);

int dPalaceDude_c::onExecute() {
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
			if (settings & 0xF000000) {
				// replicate what opening cutscene does
				SaveBlock *save = GetSaveFile()->GetBlock(-1);
				save->onWorldDataFlag(0, 1);
				SaveFileInstance->prepareSave();
				SaveFileInstance->calcCRC();
				SaveFileInstance->startNandSave();

				save->offWorldDataFlag(0, 1);
				ActivateWipe(MARIO_WIPE);
				u32 wmsettings = AssembleScWorldMapSettings(0, 0, 5);
				DoSceneChange(ProfileId::WORLD_MAP, wmsettings, 0);
			} else {
				ExitStage(ProfileId::WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
			}
			hasExitedStage = true;
		}
	}

	return true;

}

void enableWMSwitch() {
	GameMgrP->switchPalaceFlag |= (1 << 4);
}

void disableWMSwitch() {
	GameMgrP->switchPalaceFlag &= ~(1 << 4);
}

u8 getWMSwitch() {
	return (GameMgrP->switchPalaceFlag >> 4);
}
