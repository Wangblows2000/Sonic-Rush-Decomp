#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022e8be0;

typedef struct {
    s32 type;
    u32 unk4;
    u32 unk8;
    s32 unkc;
    s32 unk10;
    u8 pad14[0x54 - 0x14];
    UnkSpr_022e8be0 spr[4];
    s32 anim[4];
} UnkStruct_022e8be0;

void AnimatorSprite_SetAnimation(UnkSpr_022e8be0 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022e8be0 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022e8be0 *s);

#pragma thumb on
void ZoneMapPause_DrawItem(UnkStruct_022e8be0 *p, u32 sel) {
    s32 a;
    s32 negx;
    s32 y;
    s32 *cur;
    s32 k;
    s32 base;
    s32 idx;
    BOOL flag;
    UnkSpr_022e8be0 *spr;

    if (p->type == 3 || p->type == 7) {
        k = 2;
    } else {
        k = 1;
    }
    flag = FALSE;
    switch (sel) {
    case 0:
        base = 2;
        if (p->unk10 == 0) {
            flag = TRUE;
            base += k;
        }
        idx = 1;
        y = 0x18;
        break;
    case 1:
        base = 5;
        if (p->unk10 == 1) {
            flag = TRUE;
            base += k;
        }
        idx = 2;
        y = 0x2e;
        break;
    case 2:
        base = 8;
        if (p->unk10 == 0) {
            flag = TRUE;
            base += k;
        }
        idx = 1;
        y = 0x18;
        break;
    case 3:
        base = 11;
        if (p->unk10 == 1) {
            flag = TRUE;
            base += k;
        }
        idx = 2;
        y = 0x2e;
        break;
    }
    negx = -p->unkc;
    y -= p->unkc;
    if (flag) {
        a = 0;
        if (p->unk8 < 8) {
            a++;
        }
        if (a != p->anim[3]) {
            AnimatorSprite_SetAnimation(&p->spr[3], a);
            p->anim[3] = a;
        }
        p->spr[3].x = 8;
        p->spr[3].y = y;
        AnimatorSprite_ProcessAnimation(&p->spr[3], 0, 0);
        AnimatorSprite_Draw(&p->spr[3]);
    }
    cur = &p->anim[idx];
    if (base != *cur) {
        AnimatorSprite_SetAnimation(&p->spr[idx], base);
        *cur = base;
    }
    p->spr[idx].x = 0;
    p->spr[idx].y = negx;
    if (k == 2) {
        p->spr[idx].flags |= 4;
    } else {
        p->spr[idx].flags &= ~4;
    }
    spr = &p->spr[idx];
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}
#pragma thumb reset
