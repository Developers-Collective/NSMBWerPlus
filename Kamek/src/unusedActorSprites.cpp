#include <game.h>
#include <profile.h>

const char* WaterliftFileList [] = {"lift_han_wood", NULL};
extern "C" dActor_c *dWaterLiftBuild();
const SpriteData WaterliftSpawnerSpriteData = { ProfileId::WATER_LIFT, 0, 0, 0 , 0, 0x10, 0x10, 0, 0, 0, 0, 8};
Profile WaterliftSpawnerProfile(&dWaterLiftBuild, SpriteId::WATER_LIFT, &WaterliftSpawnerSpriteData, ProfileId::WATER_LIFT, ProfileId::EN_KERONPA, "WATER_LIFT", WaterliftFileList, 2);