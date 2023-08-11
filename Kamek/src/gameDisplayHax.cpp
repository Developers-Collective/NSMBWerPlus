#include <game.h>
#include <common.h>
#include <gameDisplay.h>
#include <stage.h>
#include <profileid.h>

extern "C" void *dAcPy_c__ChangePowerupWithAnimation(void * Player, int powerup);
extern "C" int CheckExistingPowerup(void * Player);
extern bool enableDebugMode;
bool enablePlayerDebug;
Vec playerPos;

int dGameDisplay_c::newOnExecute() {
	int orig_val = this->onExecute_orig();
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

#ifdef MOVIETESTS
	if (nowPressed & WPAD_MINUS) {
		ExitStage(ProfileId::MOVIE, OPENING_MOVIE, EXIT_LEVEL, CIRCLE_WIPE);
	}

	if (nowPressed & WPAD_B) {
		ExitStage(ProfileId::MOVIE, DEFEAT_BOWSER_MOVIE, EXIT_LEVEL, CIRCLE_WIPE);
	}

	if (nowPressed & WPAD_A) {
		ExitStage(ProfileId::MOVIE, POST_CREDIT_MOVIE, EXIT_LEVEL, CIRCLE_WIPE);
	} 
#endif
#ifndef MOVIETESTS
    if(nowPressed & WPAD_MINUS) {
		enableDebugMode = !enableDebugMode;
	}

	if(nowPressed & WPAD_B) {
		playerPos = GetSpecificPlayerActor(0)->pos;
		//enablePlayerDebug = !enablePlayerDebug; //uncomment for SUPER DEBUG MODE! ...okay, it's less of a debug mode and more of a cheat, sue me.
	}

	if(enablePlayerDebug) {
		dAcPy_c *player = dAcPy_c::findByID(0);
		int nowHeld = Remocon_GetButtons(GetActiveRemocon());
		int mult = (nowHeld & WPAD_ONE) ? 8 : 4;
		if (nowHeld & WPAD_TWO) {
			if(nowPressed & WPAD_UP) {
				int playerPowerup = ((CheckExistingPowerup(player) == 7) ? 0 : (CheckExistingPowerup(player) + 1));
				dAcPy_c__ChangePowerupWithAnimation(player, playerPowerup);
			}

			if(nowPressed & WPAD_DOWN) {
				int playerPowerup = ((CheckExistingPowerup(player) == 0) ? 7 : (CheckExistingPowerup(player) - 1));
				dAcPy_c__ChangePowerupWithAnimation(player, playerPowerup);
			}
		}
		else {
			if(nowHeld & WPAD_UP) {
				playerPos.y += mult;
			}
			else if(nowHeld & WPAD_DOWN) {
				playerPos.y -= mult;
			}

			if(nowHeld & WPAD_RIGHT) {
				playerPos.x += mult;
			}
			else if(nowHeld & WPAD_LEFT) {
				playerPos.x -= mult;
			}
		}

		GetSpecificPlayerActor(0)->pos = playerPos; 
	} 
#endif
	return orig_val;
}