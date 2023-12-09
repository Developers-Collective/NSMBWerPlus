#include <game.h>
#include <common.h>
#include <stage.h>
#include <profileid.h>

void exitToNewerCredits() {
	RESTART_CRSIN_LevelStartStruct.purpose = 0;
	RESTART_CRSIN_LevelStartStruct.world1 = 6;
	RESTART_CRSIN_LevelStartStruct.world2 = 6;
	RESTART_CRSIN_LevelStartStruct.level1 = 40;
	RESTART_CRSIN_LevelStartStruct.level2 = 40;
	RESTART_CRSIN_LevelStartStruct.areaMaybe = 0;
	RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;
	RESTART_CRSIN_LevelStartStruct.unk4 = 0; // load replay
	DontShowPreGame = true;
	DoStartLevel(&exitToNewerCredits, &RESTART_CRSIN_LevelStartStruct);

	for (int i = 0; i < 4; i++)
		if (Player_Lives[i] == 0)
			Player_Lives[i] = 5;
}