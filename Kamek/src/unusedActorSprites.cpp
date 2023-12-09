#include <game.h>
#include <profile.h>

const char* WaterliftFileList [] = {"lift_han_wood", NULL};
extern "C" dActor_c *dWaterLiftBuild();
const SpriteData WaterliftSpawnerSpriteData = { ProfileId::WATER_LIFT, 0, 0, 0 , 0, 0x10, 0x10, 0, 0, 0, 0, 8};
Profile WaterliftSpawnerProfile(&dWaterLiftBuild, SpriteId::Waterlift, &WaterliftSpawnerSpriteData, ProfileId::WATER_LIFT, ProfileId::WATER_LIFT, "WATER_LIFT", WaterliftFileList);