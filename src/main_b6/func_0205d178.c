#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 x4[4];
    s16 h8;
    s16 ha;
    u8 padc[0x3c];
} UnkSpr_0205d178;

typedef struct {
    s32 x;
    s32 y;
} UnkVec_0205d178;

typedef struct {
    UnkSpr_0205d178 spr[2];
    u8 pad90[4];
    UnkVec_0205d178 ofs[2];
    s32 xa4;
    s32 xa8;
    u8 padac[8];
    s32 xb4;
    s32 xb8;
    s16 hbc;
    s16 hbe;
} UnkGrp_0205d178;

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
} UnkState_0205d178;

extern UnkState_0205d178 gGameState;

void AnimatorSprite_DrawRotScale(void *p, s32 a, s32 b, s32 c);

static inline s32 FX_Mul_0205d178(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205d178(u8 *w) {
    UnkGrp_0205d178 *g = (UnkGrp_0205d178 *)(w + 0x324);
    u32 i;
    UnkSpr_0205d178 *s;
    u8 *o;

    if (gGameState.zone >= 7)
        return;
    s = g->spr;
    o = g->spr[0].x4;
    for (i = 0; i < 2; i++) {
        s->h8 = (g->xb4 + (g->xa4 + FX_Mul_0205d178(g->ofs[i].x, g->hbc)) - FX_Mul_0205d178(g->xb4, g->hbc)) >> 12;
        s->ha = (g->xb8 + (g->xa8 + FX_Mul_0205d178(g->ofs[i].y, g->hbe)) - FX_Mul_0205d178(g->xb8, g->hbe)) >> 12;
        AnimatorSprite_DrawRotScale(o, g->hbc, g->hbe, 0);
        s++;
        o += sizeof(UnkSpr_0205d178);
    }
}
