#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_020552bc;

typedef struct {
    s32 x;
    s32 y;
} UnkPos_020552bc;

typedef struct {
    u8 pad0[4];
    u16 flags;
    u8 pad6[2];
    u16 state;
    s16 val;
    u8 padc[0x14];
    UnkSpr_020552bc spr[4];
    UnkPos_020552bc pos[4];
    s16 unk160;
    s16 unk162;
    u8 pad164[0x12];
    u16 unk176;
} UnkWork_020552bc;

s16 ObjShift_SetFromRef(s16 v, s32 a, s32 b, s32 c, s32 d, s32 e);
s16 ObjShift_Set(s16 v, s32 a, s32 b, s32 c, s32 d);
void AnimatorSprite_Draw(UnkSpr_020552bc *s);

void func_020552bc(UnkWork_020552bc *w) {
    u16 i;

    if (w->unk176 != 0 && (w->flags & 2))
        w->flags &= ~0x18;
    switch (w->state) {
    case 0:
        w->state = 1;
        return;
    case 1:
        w->val = ObjShift_SetFromRef(w->val, 0xc00, 0, 1, 0x800, 0x200);
        if (w->val == 0xc00) {
            w->state = 2;
            w->flags &= ~0x18;
        }
        break;
    case 2:
        w->val = ObjShift_Set(w->val, 0x800, 1, 0x400, 0x200);
        break;
    case 3:
        break;
    }
    for (i = 0; i < 4; i++) {
        w->spr[i].x = w->pos[i].x >> 8;
        w->spr[i].y = w->pos[i].y >> 8;
    }
    if (!(w->flags & 4)) {
        w->spr[0].y -= w->val >> 8;
        AnimatorSprite_Draw(&w->spr[0]);
        w->spr[0].y += (w->val >> 8) * 2;
        w->spr[0].flags ^= 0x100;
        AnimatorSprite_Draw(&w->spr[0]);
        w->spr[0].flags ^= 0x100;
    }
    if (!(w->flags & 8)) {
        if (w->flags & 2)
            w->spr[1].y -= (w->val - 0x800) >> 8;
        AnimatorSprite_Draw(&w->spr[1]);
    }
    if (w->flags & 0x10)
        return;
    if (!(w->flags & 2))
        return;
    w->spr[2].y -= w->unk160;
    w->spr[3].y += w->unk162;
    if (w->unk160 > 0)
        w->unk160--;
    if (w->unk162 > 0)
        w->unk162--;
    AnimatorSprite_Draw(&w->spr[2]);
    AnimatorSprite_Draw(&w->spr[3]);
}
