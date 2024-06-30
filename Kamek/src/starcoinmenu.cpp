#include <game.h>
#include <common.h>
#include <profile.h>
#include <sfx.h>

class dCollectionCoin_c : public dBase_c {
	public:
	u8 unk[0x420-sizeof(dBase_c)];
	dStateWrapper_c<dCollectionCoin_c> state;
	USING_STATES(dCollectionCoin_c);
	REF_NINTENDO_STATE(ExitAnimeEndWait);
};

extern "C" void CollectionCoinNextWorld(int dCoinCollection);
extern "C" void CollectionCoinPreviousWorld(int dCoinCollection);

static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_ONE,WPAD_TWO,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_ONE|WPAD_TWO;
static int secretCodeIndex = 0;
static int minusCount = 0;
extern bool enableHardMode;
extern bool enableDebugMode;
extern u8 isReplayEnabled;

void addReplayAndHardMode(dCollectionCoin_c* self) {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if ((GetActiveRemocon()->heldButtons == 0x810) && (nowPressed & 0x810)) {
		if (!enableHardMode) {
			enableHardMode = true;
			OSReport("Hard Mode enabled!\n");
			MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN_HARD, 1);
		} else {
			enableHardMode = false;
			OSReport("Hard Mode disabled!\n");
		}
		return;
	}
	if (nowPressed & secretCodeButtons) {
		int nextKey = secretCode[secretCodeIndex];
		if (nowPressed & nextKey) {
			secretCodeIndex++;
			if (secretCode[secretCodeIndex] == 0) {
				secretCodeIndex = 0;
				if (isReplayEnabled != 100) {
					isReplayEnabled = 100;
					MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CS_COURSE_IN, 1);
					OSReport("Replay Recording enabled!\n");
				} else {
					isReplayEnabled = 0;
					OSReport("Replay Recording disabled!\n");
				}
			}
			return;
		} else {
			secretCodeIndex = 0;
		}
	}
	if (nowPressed & WPAD_MINUS) {
		minusCount++;
		if (minusCount >= 16) {
			minusCount = 0;

			enableDebugMode = !enableDebugMode;

			if (enableDebugMode) {
				MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_GET_PRIZE, 1);
				OSReport("CollisionDebug enabled!\n");
			} else {
				OSReport("CollisionDebug disabled!\n");
			}
		}
	} else if (nowPressed & WPAD_RIGHT) {
		CollectionCoinNextWorld((int)self);
	} else if (nowPressed & WPAD_LEFT) {
		CollectionCoinPreviousWorld((int)self);
	} else if (nowPressed & WPAD_ONE) {
		self->state.setState(&dCollectionCoin_c::StateID_ExitAnimeEndWait);
	}
}