#include <game.h>
#include <profile.h>

// 80a6a1c0
void newSpawnGoombaFromMegaGoomba(dEn_c *megaGoomba) {
    Vec childPos = megaGoomba->pos;
    u32 spriteTexValue = megaGoomba->settings >> 24 & 0xF;
    u32 backFenceThing = megaGoomba->appearsOnBackFence << 0x10;
    childPos.x += 4.0;
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 4, &childPos, 0, 0);
    childPos.x -= 8.0;
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x100004, &childPos, 0, 0);
}

// 80a6a260
void newSpawnGoombaFromMegaGoomba2(dEn_c *megaGoomba) {
    Vec childPos = megaGoomba->pos;
    u32 spriteTexValue = megaGoomba->settings >> 24 & 0xF;
    u32 backFenceThing = megaGoomba->appearsOnBackFence << 0x10;
    childPos.x += 4.0;
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 5, &childPos, 0, 0);
    childPos.x -= 8.0;
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x100005, &childPos, 0, 0);
}

// 80a5aa60
void newSpawnMegaGoombaFromLargeGoomba(dEn_c *largeGoomba) {
    Vec childPos = largeGoomba->pos;
    u32 spriteTexValue = largeGoomba->settings >> 24 & 0xF;
    u32 backFenceThing = largeGoomba->appearsOnBackFence << 0x10;
    childPos.x += 9.0;
    dStageActor_c::create(ProfileId::EN_MIDDLE_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x10000000, &childPos, 0, 0);
    childPos.x -= 18.0;
    dStageActor_c::create(ProfileId::EN_MIDDLE_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x10100000, &childPos, 0, 0);
}

// 80a5aaf0
void newSpawnGoombaFromLargeGoomba(dEn_c *largeGoomba) {
    Vec childPos[4];
    childPos[0] = (Vec){largeGoomba->pos.x + 8.0, largeGoomba->pos.y + 16.0, largeGoomba->pos.z};
    childPos[1] = (Vec){largeGoomba->pos.x - 8.0, largeGoomba->pos.y + 16.0, largeGoomba->pos.z};
    childPos[2] = (Vec){largeGoomba->pos.x + 8.0, largeGoomba->pos.y, largeGoomba->pos.z};
    childPos[3] = (Vec){largeGoomba->pos.x - 8.0, largeGoomba->pos.y, largeGoomba->pos.z};
    u32 spriteTexValue = largeGoomba->settings >> 24 & 0xF;
    u32 backFenceThing = largeGoomba->appearsOnBackFence << 0x10;
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 5, &childPos[0], 0, 0);
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x100005, &childPos[1], 0, 0);
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x200005, &childPos[2], 0, 0);
    dStageActor_c::create(ProfileId::EN_KURIBO, backFenceThing | (spriteTexValue << 24) | 0x300005, &childPos[3], 0, 0);
}