// Alright, so you might be wondering what this is...
// This is essentially the early pregame you see in early newer videos, just remade to work with modern levelinfo
// You'll need to make the layout yourself though... it's pretty simple, just add a TXT_WorldName and TXT_LevelName textbox in your pregame layout.
#include <common.h>
#include <game.h>
#include <profileid.h>
#include <stage.h>
#include "levelinfo.h"
#include "fileload.h"
#include "layoutlib.h"

extern char CurrentLevel;
extern char CurrentWorld;

int DoNames(int state) {
	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;
	const char *levelname;
	const char *worldname;

	// Skip the title screen
	// and only process the code if the State is set to 1
	// (the screen has been initialised)
	if (state == 1 && lnum != STAGE_TITLE) {
		// grab the CRSIN object
		fBase_c *ptr = fBase_c::searchByProfileId(ProfileId::CRSIN, 0);

		// FIX !!!!!

		if (ptr != 0) {
			void *worldObj = EmbeddedLayout_FindTextBoxByName((Layout*)((u32)ptr+0xB0), "TXT_WorldName");
			void *levelObj = EmbeddedLayout_FindTextBoxByName((Layout*)((u32)ptr+0xB0), "TXT_LevelName");
			if (worldObj == 0 || levelObj == 0) return state;

			dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
			dLevelInfo_c::entry_s *world = dLevelInfo_c::s_info.searchByDisplayNum(CurrentWorld+1, 100);
			if (level) {
				levelname = dLevelInfo_c::s_info.getNameForLevel(level);
			} else {
				levelname = "Not found in LevelInfo!";
			}
			if (world) {
				worldname = dLevelInfo_c::s_info.getNameForLevel(world);
			} else {
				worldname = "Not found in LevelInfo!";
			}
			void *vtable = *((void**)levelObj);
			void *funcaddr = *((void**)((u32)vtable+0x7C));
			int (*SetString)(void*, unsigned short*, unsigned short);
			SetString = (int(*)(void*, unsigned short*, unsigned short))funcaddr;

			unsigned short wbuffer[0x40], lbuffer[0x40];
			for (int i = 0; i < 0x40; i++) {
				wbuffer[i] = (unsigned short)worldname[i];
				lbuffer[i] = (unsigned short)levelname[i];
			}

			SetString(worldObj, wbuffer, 0);
			SetString(levelObj, lbuffer, 0);
		}
	} else {
	}

	return state;
}
