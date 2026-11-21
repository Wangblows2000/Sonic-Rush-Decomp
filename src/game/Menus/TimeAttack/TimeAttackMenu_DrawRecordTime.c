#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022e9fe4;

typedef struct {
    s32 x;
    s32 unk4;
    s32 y;
    s32 idx;
    s32 digits[5];
    s32 flag;
} UnkEntry_022e9fe4;

typedef struct {
    u8 pad0[8];
    UnkEntry_022e9fe4 entries[13];
    u8 pad[0x228 - 8 - 13 * 0x28];
    UnkSpr_022e9fe4 spr[20];
} UnkStruct_022e9fe4;

void AnimatorSprite_ProcessAnimation(UnkSpr_022e9fe4 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022e9fe4 *s);

#define DRAW(dx, dy)                  \
    spr->x = e->x + (dx);             \
    spr->y = e->y + (dy);             \
    AnimatorSprite_ProcessAnimation(spr, 0, 0);         \
    AnimatorSprite_Draw(spr);

#pragma thumb on
void TimeAttackMenu_DrawRecordTime(UnkStruct_022e9fe4 *p, s32 i) {
    UnkEntry_022e9fe4 *e = &p->entries[i];
    UnkSpr_022e9fe4 *spr;

    spr = &p->spr[e->idx + 5];
    if (e->flag) {
        DRAW(0x50, 1)
    }
    spr = &p->spr[e->idx + 0x1a];
    if (e->flag) {
        DRAW(0x78, 7)
    }
    if (e->flag) spr = &p->spr[e->digits[0] + 7]; else spr = &p->spr[17];
    DRAW(0xe8, 4)
    if (e->flag) spr = &p->spr[e->digits[1] + 7]; else spr = &p->spr[17];
    DRAW(0xe0, 4)
    if (e->flag) spr = &p->spr[e->digits[2] + 7]; else spr = &p->spr[17];
    DRAW(0xd0, 4)
    if (e->flag) spr = &p->spr[e->digits[3] + 7]; else spr = &p->spr[17];
    DRAW(0xc8, 4)
    if (e->flag) spr = &p->spr[e->digits[4] + 7]; else spr = &p->spr[17];
    DRAW(0xb8, 4)
    spr = &p->spr[18];
    DRAW(0xc0, 4)
    spr = &p->spr[19];
    DRAW(0xd8, 4)
}
#pragma thumb reset
