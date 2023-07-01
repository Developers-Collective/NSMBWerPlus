
#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "gameDisplay.h"

extern u8 doWait[4];
extern bool enableDebugMode;

extern "C" int CheckExistingPowerup(void * Player);

int dGameDisplay_c::doWaitCheck() {

	dAcPy_c *player = dAcPy_c::findByID(0);
	int p = CheckExistingPowerup(player);
	if(player->input.nowPressed & WPAD_MINUS) {
		enableDebugMode = !enableDebugMode;
	}
	
}
