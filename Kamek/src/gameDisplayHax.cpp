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
static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_ONE,WPAD_TWO,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_ONE|WPAD_TWO;
static int secretCodeIndex = 0;

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
/*	if (nowPressed & secretCodeButtons) { //more debug crap
		int nextKey = secretCode[secretCodeIndex];
		if (nowPressed & nextKey) {
			secretCodeIndex++;
			if (secretCode[secretCodeIndex] == 0) {
				secretCodeIndex = 0;
				SaveFile *file = GetSaveFile();
				SaveBlock *block = file->GetBlock(file->header.current_file);
				OSReport("Here's your items you sissy!\n");
				int i = 1;
				for (int i = 0; i < 9; i++) {
					block->new_powerups_available[i] = 99;
				};
			}
			return;
		} else {
			secretCodeIndex = 0;
		}
	}
*/
#endif
	return orig_val;
}