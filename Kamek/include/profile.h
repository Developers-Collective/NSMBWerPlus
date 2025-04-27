#ifndef __KAMEK_PROFILE_H
#define __KAMEK_PROFILE_H

#if defined(REGION_KW) || defined(REGION_C)
	#define ORIGINAL_PROFILES 752
#else
	#define ORIGINAL_PROFILES 750
#endif


#include <game.h>
#include <profileid.h>

// typedef struct { f32 x, y, z, w; } Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

struct SpriteData
{
    u16 profileId;
    //u8 pad[2];
    s32 xoffs;
    s32 yoffs;
    // Quaternion spawnRange;
    s32 spawnRangeOffset[2];
    s32 spawnRangeSize[2];
    u16 _1C;
    u16 _1E;
    u16 _20;
    u16 _22;
	/* Values:
    0x2: always spawn even if offscreen
    0x8: something with z-order
    0x10: grouped sprite */
    u16 flags;
    //u8 pad2[2];
};

class Profile
{
public:
    Profile(dActor_c* (*buildFunc)(), u32 spriteId, const SpriteData* spriteData, u16 executeOrderProfileId, u16 drawOrderProfileId, const char* name, const char** files=NULL, u32 flags = 0);

    dActor_c* (*buildFunc)();
    u16 executeOrderProfileId;
    u16 drawOrderProfileId;
	/* Values:
	0x2: unknown, several profiles have this set
	0x4: unknown, never used to my knowledge
	0x8: makes Mario look at the actor
	0x10: killed by level clear and awards score
	0x20: killed by level clear
	0x40: only set on balloons??
	0x80: can be killed by iceballs (magic/podobooos/etc)
	0x200: bounces Penguin Mario back when sliding into the actor
	0x400: ignores wrap around edges */
	u32 flags;
};

extern "C" SpriteData originalSprites[483];
extern "C" SpriteData sprites[];

extern "C" Profile* originalProfiles[ORIGINAL_PROFILES];
extern "C" Profile* profiles[];


#endif