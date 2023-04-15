#include <profile.h>

#define ORIGINAL_PROFILES 750

/* SPRITES */

SpriteData sprites[SpriteId::Num] = { 0 };

class SpritesSetter
{
public:
    SpritesSetter();
};

SpritesSetter::SpritesSetter()
{
    for (u32 i = 0; i < 483; i++)
        sprites[i] = originalSprites[i];
}

static SpritesSetter doSetSprites;

/* PROFILES */

Profile* profiles[ProfileId::Num] = { 0 };
extern Profile** ObjectProfileList;

class ProfileSetter
{
public:
    ProfileSetter();
};

ProfileSetter::ProfileSetter()
{
    ObjectProfileList = &profiles[0];

    for (u32 i = 0; i < 750; i++)
        profiles[i] = originalProfiles[i];
}

static ProfileSetter doSetProfiles;

void SetObjectProfileList()
{
    ObjectProfileList = &profiles[0];
}

/* SPRITE FILES */

extern const char** spriteFiles[483];
const char** customSpriteFiles[SpriteId::Num - 483] = { 0 };

/* PROFILE NAMES*/

extern const char* profileNames[750];
const char* customProfileNames[ProfileId::Num - 750] = { 0 };

const char* getProfileName(u32 profileId)
{
    if (profileId < 750)
        return profileNames[profileId];

    return customProfileNames[profileId - 750];
}

/* CUSTOM PROFILE CTOR */

Profile::Profile(dActor_c* (*buildFunc)(), u32 id, const SpriteData* spriteData, u16 executeOrderProfileId, u16 drawOrderProfileId, const char* name, const char** files, u32 flags)
{
    this->buildFunc = buildFunc;
    this->executeOrderProfileId = executeOrderProfileId;
    this->drawOrderProfileId = drawOrderProfileId;
	this->flags = flags;

	u32 profileId;
	if (spriteData) {
		sprites[id] = *spriteData;
		if (id < 483) {
			spriteFiles[id] = files;
		} else {
			customSpriteFiles[id - 483] = files;
		}
		profileId = spriteData->profileId;
	} else {
		profileId = id;
	}
	
	profiles[profileId] = this;
	if (profileId < ORIGINAL_PROFILES) {
		profileNames[profileId] = name;
	} else {
		customProfileNames[profileId - ORIGINAL_PROFILES] = name;
	}
}