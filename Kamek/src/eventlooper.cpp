#include <common.h>
#include <game.h>
#include <profile.h>

class EventLooper : public dStageActor_c {
    u64 eventFlag;	// 0x3D0
	u64 eventActive;	// 0x3D0
	u8 delay;		// 0x3D4
	u8 delayCount;	// 0x3D7

    int onCreate();
    int onExecute();

    public: static dActor_c *build();
};

const SpriteData EventLooperSpriteData = {ProfileId::AC_EVENT_LOOPER, 8, 0xFFFFFFF0, 0, 0x10, 8, 0x10, 0, 0, 0, 0, 2};
// #      -ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
Profile EventLooperProfile(&EventLooper::build, SpriteId::AC_EVENT_LOOPER, &EventLooperSpriteData, ProfileId::EN_HELPOS, ProfileId::AC_EVENT_LOOPER, "AC_EVENT_LOOPER");

dActor_c* EventLooper::build() {
    void* buffer = AllocFromGameHeap1(sizeof(EventLooper));
    return new(buffer) EventLooper;
}

int EventLooper::onCreate() {
    char eventStart	= (this->settings >> 24)	& 0xFF;
	char eventEnd	= (this->settings >> 16)	& 0xFF;

	// Putting all the events into the flag
	int i;
	u64 q = (u64)0;
	for(i=eventStart;i<(eventEnd+1);i++)
	{
		q = q | ((u64)1 << (i - 1));
	}
		
	this->eventFlag = q;
	
	this->delay		= (((this->settings) & 0xFF) + 1) * 10;
	this->delayCount = 0;
	
	char tmpEvent= (this->settings >> 8)	& 0xFF;
	if (tmpEvent == 0)
	{
		this->eventActive = (u64)0xFFFFFFFFFFFFFFFF;
	}
	else
	{
		this->eventActive = (u64)1 << (tmpEvent - 1);
		
	}
	

	if (dFlagMgr_c::instance->flags & this->eventActive)
	{
		u64 evState = (u64)1 << (eventStart - 1);
		dFlagMgr_c::instance->flags |= evState;
	}

	onExecute();
	
	return true;
}

int EventLooper::onExecute() {
    if ((dFlagMgr_c::instance->flags & this->eventActive) == 0)
		return true;

	// Waiting for the right moment
	if (this->delayCount < this->delay) 
	{

		this->delayCount = this->delayCount + 1;
		return true;
	}	
	
	// Reset the delay
	this->delayCount = 0;
	
	// Find which event(s) is/are on
	u64 evState = dFlagMgr_c::instance->flags & this->eventFlag;
	
	// Turn off the old events
	dFlagMgr_c::instance->flags = dFlagMgr_c::instance->flags & (~this->eventFlag);
	
	// Shift them right if they can, if not, reset!
	evState = evState << 1;
	if (evState < this->eventFlag)
	{
		dFlagMgr_c::instance->flags = dFlagMgr_c::instance->flags | evState;
	}
	
	else
	{
		char eventStart	= (this->settings >> 24)	& 0xFF;
		evState = (u64)1 << (eventStart - 1);
		dFlagMgr_c::instance->flags = dFlagMgr_c::instance->flags | evState;
	}

    return true;
}