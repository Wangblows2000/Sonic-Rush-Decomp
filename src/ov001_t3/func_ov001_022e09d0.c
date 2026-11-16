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
} UnkSpr_022e09d0;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 count;
    u8 pad10[0x68 - 0x10];
    s32 recs[(0xd8 - 0x68) / 0xc][3];
    u8 padd4[0xd8 - 0x68 - ((0xd8 - 0x68) / 0xc) * 0xc];
    s32 unkd8;
    u8 paddc[0x360 - 0xdc];
    UnkSpr_022e09d0 spr[(0xe10 - 0x360) / 0x48];
    s32 anim[16];
} UnkStruct_022e09d0;

extern s32 data_ov001_0231c3ec;
extern s32 gZoneActSelectResult;

void AnimatorSprite_SetAnimation(UnkSpr_022e09d0 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022e09d0 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022e09d0 *s);

#pragma thumb on
void func_ov001_022e09d0(UnkStruct_022e09d0 *p) {
    u16 prio;
    s32 i;
    UnkSpr_022e09d0 *s;
    s32 anim;
    BOOL vis;

    for (i = 0; i < 3; i++) {
        s = &p->spr[i + 13];
        anim = p->recs[data_ov001_0231c3ec][i] * 3 + 0x1a;
        if (i == gZoneActSelectResult) {
            if (p->unkd8 != 0 && p->unk8 < 0x3c) {
                anim += 2;
                prio = 12;
                vis = TRUE;
            } else {
                anim++;
                prio = 11;
                vis = FALSE;
            }
        } else {
            prio = 10;
            vis = FALSE;
        }
        if (anim != p->anim[i + 13]) {
            AnimatorSprite_SetAnimation(s, anim);
            p->anim[i + 13] = anim;
        }
        s->unk44 = prio;
        if (vis) {
            s->flags |= 4;
        } else {
            s->flags &= ~4;
        }
        s->x = 0xa9;
        s->y = i * 0x20 + 0x4f;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
