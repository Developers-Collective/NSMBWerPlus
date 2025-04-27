#include <common.h>
#include <game.h>
#include <dCourse.h>
#include <profile.h>

class LevelSpecial : public dStageActor_c {
public:
	u64 eventFlag;
	u8 type;
	u8 effect;
	u8 lastEvState;
	u8 func;
	u32 keepTime;
	u32 setTime;

	int onCreate();
	int onExecute();

	static dActor_c* build();

};

extern u16 TimeStopFlag;
extern u32 AlwaysDrawFlag;
extern u32 AlwaysDrawBranch;

extern float MarioDescentRate;
extern float MarioJumpMax;
extern float MarioJumpArc;
extern float MiniMarioJumpArc;
// extern float MarioSize;

extern float GlobalSpriteSize;
extern float GlobalSpriteSpeed;
extern float GlobalRiderSize;
extern char SizerOn;
extern char ZOrderOn;
extern int GlobalStarsCollected;

extern VEC2 BGScaleFront;
extern VEC2 BGScaleBack;
extern char BGScaleEnabled;

extern u32 GameTimer;

#define time *(u32*)((GameTimer) + 0x4)


static const float GlobalSizeFloatModifications [] = {1, 0.25, 0.5, 0.75, 1.25, 1.5, 1.75, 2, 2.5, 3, 4, 5, 6, 7, 8, 10 };
static const float GlobalRiderFloatModifications [] = {1, 0.6, 0.7, 0.9, 1, 1, 1, 1.1, 1.25, 1.5, 2, 2.5, 3, 3.5, 4, 5};
static const float BGScaleChoices[] = {0.1f, 0.15f, 0.25f, 0.375f, 0.5f, 0.625f, 0.75f, 0.9f, 1.0f, 1.125f, 1.25f, 1.5f, 1.75f, 2.0f, 2.25f, 2.5f};

bool NoMichaelBuble = false;

bool ResetAfterLevel();

#define ACTIVATE	1
#define DEACTIVATE	0

extern "C" void dAcPy_vf294(void *Mario, dStateBase_c *state, u32 unk);
void MarioStateChanger(void *Mario, dStateBase_c *state, u32 unk) {
	//OSReport("State: %p, %s", state, state->getName());

	if ((strcmp(state->getName(), "dAcPy_c::StateID_Balloon") == 0) && (NoMichaelBuble)) { return; }

	dAcPy_vf294(Mario, state, unk);
}

bool ResetAfterLevel(bool didItWork) {
	// TimeStopFlag = 0;
	MarioDescentRate = -4;
	MarioJumpMax = 3.628;
	MarioJumpArc = 2.5;
	MiniMarioJumpArc = 2.5;
	// MarioSize = 1.0;
	GlobalSpriteSize = 1.0;
	GlobalSpriteSpeed = 1.0;
	GlobalRiderSize = 1.0;
	SizerOn = 0;
	AlwaysDrawFlag = 0x9421FFF0;
	AlwaysDrawBranch = 0x7C0802A6;
	ZOrderOn = 0;
	GlobalStarsCollected = 0;
	NoMichaelBuble = false;
	BGScaleEnabled = 0;
	return didItWork;
}

void FuckinBubbles() {
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());
	bool thing = false;

	int zone = GetZoneNum();
	for (int i = 0; i < course->zoneSpriteCount[zone]; i++) {
		dCourse_c::sprite_s *spr = &course->zoneFirstSprite[zone][i];
		if (spr->type == 246 && (spr->settings & 0xF) == 8)
			thing = true;
	}

	if (thing) {
		OSReport("DISABLING EXISTING BUBBLES.\n");
		for (int i = 0; i < 4; i++)
			Player_Flags[i] &= ~4;
	}
}

dActor_c* LevelSpecial::build() {
	void *buffer = AllocFromGameHeap1(sizeof(LevelSpecial));
	return new(buffer) LevelSpecial;
}

const SpriteData LevelSpecialSpriteData = { ProfileId::AC_SPECIAL_EVENT, 0, 0, 0, 0, 0x8, 0x8, 0, 0, 0, 0, 2 };
Profile LevelSpecialProfile(&LevelSpecial::build, SpriteId::AC_SPECIAL_EVENT, &LevelSpecialSpriteData, ProfileId::TAG_THUNDER, ProfileId::AC_SPECIAL_EVENT, "AC_SPECIAL_EVENT");


int LevelSpecial::onCreate() {
    char eventNum	= (this->settings >> 24)	& 0xFF;
	this->eventFlag = (u64)1 << (eventNum - 1);
	
	this->keepTime  = 0;
	
	this->type		= (this->settings)			& 15;
	this->effect	= (this->settings >> 4)		& 15;
	this->setTime	= (this->settings >> 8)     & 0xFFFF;

	this->lastEvState = 0xFF;

	this->onExecute();

	return true;
}

int LevelSpecial::onExecute() {
	if (this->keepTime > 0) {
		time = this->keepTime; }
    
	u8 newEvState = 0;
	if (dFlagMgr_c::instance->flags & this->eventFlag)
		newEvState = 1;
	
	if (newEvState == this->lastEvState)
		return true;
		
	
	u8 offState;
	if (newEvState == ACTIVATE)
	{
		offState = (newEvState == 1) ? 1 : 0;

		switch (this->type) {
			// case 1:											// Time Freeze
			// 	TimeStopFlag = this->effect * 0x100;
			// 	break;
				
			case 2:											// Stop Timer
				this->keepTime  = time;
				break;
		
	
			case 3:											// Mario Gravity
				if (this->effect == 0)
				{											//Low grav
					MarioDescentRate = -2;
					MarioJumpArc = 0.5;
					MiniMarioJumpArc = 0.5;
					MarioJumpMax = 4.5;
				}
				else
				{											//Anti-grav
					MarioDescentRate = 0.5;
					MarioJumpArc = 4.0;
					MiniMarioJumpArc = 4.0;
					MarioJumpMax = 0.0;
				}
				break;
	
			case 4:											// Set Time
				time = (this->setTime << 0xC) - 1; // Possibly - 0xFFF?
				break;


			case 5:											// Global Enemy Size
				SizerOn = 3;

				GlobalSpriteSize = GlobalSizeFloatModifications[this->effect];
				GlobalRiderSize = GlobalRiderFloatModifications[this->effect];
				GlobalSpriteSpeed = GlobalRiderFloatModifications[this->effect];

				AlwaysDrawFlag = 0x38600001;
				AlwaysDrawBranch = 0x4E800020;
				break;
	
			case 6:											// Individual Enemy Size
				AlwaysDrawFlag = 0x38600001;
				AlwaysDrawBranch = 0x4E800020;

				if (this->effect == 0)
				{	
					SizerOn = 1;							// Nyb 5
				}
				else
				{											
					SizerOn = 2;							// Nyb 7
				}
				break;
		
			case 7:											// Z Order Hack
				ZOrderOn = 1;
				break;

			case 8:
				NoMichaelBuble = true;
				break;

			case 9:
				BGScaleEnabled = true;
				BGScaleFront.x = BGScaleChoices[(this->settings >> 20) & 15];
				BGScaleFront.y = BGScaleChoices[(this->settings >> 16) & 15];
				BGScaleBack.x = BGScaleChoices[(this->settings >> 12) & 15];
				BGScaleBack.y = BGScaleChoices[(this->settings >> 8) & 15];
				break;

			default:
				break;
		}
	}
	
	else
	{
		offState = (newEvState == 1) ? 0 : 1;

		switch (this->type) {
			// case 1:											// Time Freeze
			// 	TimeStopFlag = 0;
			// 	break;
				
			case 2:											// Stop Timer
				this->keepTime  = 0;
				break;
		
	
			case 3:											// Mario Gravity
				MarioDescentRate = -4;
				MarioJumpArc = 2.5;
				MiniMarioJumpArc = 2.5;
				MarioJumpMax = 3.628;
				break;
	
			case 4:											// Mario Size
				break;
		
			case 5:											// Global Enemy Size
				SizerOn = 0;

				GlobalSpriteSize = 1.0;
				GlobalRiderSize = 1.0;
				GlobalSpriteSpeed = 1.0;

				AlwaysDrawFlag = 0x9421FFF0;
				AlwaysDrawBranch = 0x7C0802A6;
				break;

			case 6:											// Individual Enemy Size
				SizerOn = 0;

				AlwaysDrawFlag = 0x9421FFF0;
				AlwaysDrawBranch = 0x7C0802A6;
				break;
		
			case 7:											// Z Order Hack
				ZOrderOn = 0;
				break;
				
			case 8:
				NoMichaelBuble = false;
				break;

			case 9:
				BGScaleEnabled = false;
				break;
	
			default:
				break;
		}
	}



	
	
	this->lastEvState = newEvState;
	return true;
}