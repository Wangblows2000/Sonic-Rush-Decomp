#include <nitro/types.h>

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x24];
    u32 unk2c;
    u8 pad30[0x14];
    u16 unk44;
    u8 pad46[2];
} UnkSpr_0204481c;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_0204481c spr[1];
} UnkObj_0204481c;

typedef struct {
    s32 unk0;
    UnkObj_0204481c *obj;
    u16 unk8;
    u16 unka;
    u16 unkc;
} UnkWork_0204481c;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204481c *work;
} UnkTask_0204481c;

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
    u16 unk22;
} UnkDisp_0204481c;

typedef struct {
    u8 pad0[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkTask_0204481c *gTaskCurrent;
extern UnkDisp_0204481c *gDisplayTable[];
extern UnkStruct_02369ac0 gBossStatus;
extern u16 data_02073264[];
extern s16 data_02073274[];
extern s16 data_02073276[];

void func_02044ad0(void);
void G2x_SetBlendAlpha_(vu32 *p, u32 a, u32 b, u32 c, u32 d);
void AnimatorSprite_DrawRotScale(UnkSpr_0204481c *s, s32 sx, s32 sy, s32 r);
void NaviB_DrawIcons(UnkObj_0204481c *o, s32 idx, u16 c);

static inline s32 FX_Mul_0204481c(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

#define CLAMP_0204481c(x) ((x) < 0 ? 0 : ((x) > 0x1000 ? 0x1000 : (x)))

static inline s32 Lerp_0204481c(s32 v, s32 to, s16 t, s32 n) {
    do {
        v += FX_Mul_0204481c(to - v, t);
    } while (n--);
    return v;
}

void func_0204481c(void) {
    UnkWork_0204481c *w;
    UnkObj_0204481c *obj;
    UnkSpr_0204481c *spr;
    s32 idx;
    s32 v;
    s32 sx;
    s32 sy;

    w = gTaskCurrent->work;
    idx = w->unk8 - 1;
    obj = w->obj;
    w->unk0 += 0x100;
    if (w->unk0 >= 0x1000) {
        gTaskCurrent->fn = func_02044ad0;
        w->unk8 = gBossStatus.unk8;
        G2x_SetBlendAlpha_((vu32 *)0x4001050, 0, 0, 0x1f, 0x1f);
        gDisplayTable[1]->unk20 = 0x40;
        gDisplayTable[1]->unk22 = 0x1f1f;
    } else {
        v = Lerp_0204481c(0x1f, 0, CLAMP_0204481c(w->unk0), 3);
        if (v != 0) {
            gDisplayTable[1]->unk22 = v | ((0x1f - v) << 8);
            spr = &obj->spr[data_02073264[idx]];
            sx = Lerp_0204481c(0x1000, 0x1199, CLAMP_0204481c(w->unk0), 3);
            sy = Lerp_0204481c(0x1000, 0x2000, CLAMP_0204481c(w->unk0), 3);
            spr->unk44 = 8;
            spr->unk2c |= 0x600;
            spr->unk4 = data_02073274[idx * 2];
            spr->unk6 = data_02073276[idx * 2];
            AnimatorSprite_DrawRotScale(spr, sx, sy, 0);
            spr->unk2c &= ~0x600;
            spr->unk44 = 6;
        }
    }
    w->unka++;
    if (w->unka > 2) {
        w->unkc ^= 1;
        w->unka = 0;
    }
    NaviB_DrawIcons(obj, idx, w->unkc + 6);
}
