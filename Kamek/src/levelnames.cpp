// Alright, so you might be wondering what this is...
// This is essentially the early pregame you see in early newer videos, just remade to work with modern levelinfo
// You'll need to make the layout yourself though... it's pretty simple, just add a TXT_WorldName and TXT_LevelName textbox in your pregame layout.
#include <common.h>
#include <game.h>
#include <profileid.h>
#include <stage.h>
#include "levelinfo.h"

extern char CurrentLevel;
extern char CurrentWorld;

int DoNames(int state) {
	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;
	const char *levelname;
	const char *worldname;
	nw4r::lyt::TextBox
		*worldObj, *levelObj;
	// Skip the title screen
	// and only process the code if the State is set to 1
	// (the screen has been initialised)
	if (state == 1 && lnum != STAGE_TITLE) {
		// grab the CRSIN object
		fBase_c *ptr = fBase_c::searchByProfileId(ProfileId::CRSIN, 0);

		// FIX !!!!!

		if (ptr != 0) {
			m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)((u32)ptr+0xB0);
			worldObj = layout->findTextBoxByName("TXT_WorldName");
			levelObj = layout->findTextBoxByName("TXT_LevelName");
			if (worldObj == 0 || levelObj == 0) return state;

			dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
			dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
			if (level) {
				levelname = dLevelInfo_c::s_info.getNameForLevel(level);
			} else {
				char levelNumber[15];
				sprintf(levelNumber, "%d-%d (UNNAMED)", wnum+1, lnum+1);
				levelname = levelNumber;
			}
			if (world) {
				worldname = dLevelInfo_c::s_info.getNameForLevel(world);
			} else {
				char worldNumber[8];
				sprintf(worldNumber, "World %d", wnum+1);
				worldname = worldNumber;
			}

			wchar_t wbuffer[0x40], lbuffer[0x40];
			for (int i = 0; i < 0x40; i++) {
				wbuffer[i] = (unsigned short)worldname[i];
				lbuffer[i] = (unsigned short)levelname[i];
			}

			worldObj->SetString(wbuffer);
			levelObj->SetString(lbuffer);
		}
	} else {
	}

	return state;
}