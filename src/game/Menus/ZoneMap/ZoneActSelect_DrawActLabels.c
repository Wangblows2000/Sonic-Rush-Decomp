#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u8 pad46[2];
} UnkSpr_022e0c5c;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 count;
    u8 pad10[0xd8 - 0x10];
    s32 unkd8;
    u8 paddc[0x360 - 0xdc];
    UnkSpr_022e0c5c spr[(0xe10 - 0x360) / 0x48];
    s32 anim[12];
} UnkStruct_022e0c5c;

extern s32 gZoneActSelectResult;

void AnimatorSprite_SetAnimation(UnkSpr_022e0c5c *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022e0c5c *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022e0c5c *s);

#pragma thumb on
void ZoneActSelect_DrawActLabels(UnkStruct_022e0c5c *p) {
    s32 i;
    UnkSpr_022e0c5c *s;
    s32 anim;
    u16 prio;
    BOOL vis;

    for (i = 0; i < 3; i++) {
        s = &p->spr[i + 9];
        anim = i * 3 + 0xf;
        if (i == gZoneActSelectResult) {
            if (p->unkd8 != 0 && p->unk8 < 0x3c) {
                anim += 2;
                prio = 6;
                vis = TRUE;
            } else {
                anim++;
                prio = 5;
                vis = FALSE;
            }
        } else {
            prio = 4;
            vis = FALSE;
        }
        if (anim != p->anim[i + 9]) {
            AnimatorSprite_SetAnimation(s, anim);
            p->anim[i + 9] = anim;
        }
        s->unk44 = prio;
        if (vis) {
            s->flags |= 4;
        } else {
            s->flags &= ~4;
        }
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
