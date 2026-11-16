#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_0205d278;

typedef struct {
    s32 x;
    s32 y;
} UnkPt_0205d278;

typedef struct {
    u8 pad0[4];
    UnkSpr_0205d278 spr[2];
    UnkPt_0205d278 pt[2];
    s32 cx;
    s32 cy;
    u8 padAC[8];
    s32 ox;
    s32 oy;
    s16 sx;
    s16 sy;
} UnkSub_0205d278;

typedef struct {
    u8 pad0[0x260];
    UnkSub_0205d278 sub;
} UnkWork_0205d278;

void AnimatorSprite_DrawRotScale(UnkSpr_0205d278 *s, s32 sx, s32 sy, u16 r);

static inline s32 FX_Mul_0205d278(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205d278(UnkWork_0205d278 *w) {
    UnkSub_0205d278 *s = &w->sub;
    u32 i;

    for (i = 0; i < 2; i++) {
        s->spr[i].x = (s->ox + (s->cx + FX_Mul_0205d278(s->pt[i].x, s->sx)) - FX_Mul_0205d278(s->ox, s->sx)) >> 12;
        s->spr[i].y = (s->oy + (s->cy + FX_Mul_0205d278(s->pt[i].y, s->sy)) - FX_Mul_0205d278(s->oy, s->sy)) >> 12;
        AnimatorSprite_DrawRotScale(&s->spr[i], s->sx, s->sy, 0);
    }
}
