#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    u8 pad8[8];
    u32 flags;
} UnkWork_022e129c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e129c *work;
} UnkTask_022e129c;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022e129c;

extern UnkTask_022e129c *gTaskCurrent;
extern UnkBright_022e129c data_0208926c;
extern UnkBright_022e129c data_02087d44;
extern s32 data_ov001_0231c3ec;
extern UnkWork_022e129c *gZoneActSelectWork;

s32 FX_Div(s32 n, s32 d);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void ZoneActSelect_Unk_ov001_022e08e0(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e09d0(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0ad0(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0c5c(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0dfc(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0d30(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0884(UnkWork_022e129c *w);
void ZoneActSelect_Unk_ov001_022e0858(UnkWork_022e129c *w, s32 a);
void NaviM_Unk_02064674(void);
void NaviM_Unk_02064788(void);
void ZoneActSelect_State_Select(void);

#pragma thumb on
static inline fx32 FX_Mul_022e129c(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void ZoneActSelect_State_FadeIn(void) {
    UnkWork_022e129c *w;
    s32 v;

    w = gTaskCurrent->work;
    v = (FX_Div(FX_Mul_022e129c(w->timer << 12, -0x10000), 0x1e000) + 0x10000 + 0x800) >> 12;
    if (v < -0x10) v = -0x10;
    if (v > 0x10) v = 0x10;
    data_0208926c.bright = v;
    if (w->flags & 1) {
        data_02087d44.bright = v;
    }
    ZoneActSelect_Unk_ov001_022e08e0(w);
    ZoneActSelect_Unk_ov001_022e09d0(w);
    ZoneActSelect_Unk_ov001_022e0ad0(w);
    ZoneActSelect_Unk_ov001_022e0c5c(w);
    ZoneActSelect_Unk_ov001_022e0dfc(w);
    ZoneActSelect_Unk_ov001_022e0d30(w);
    ZoneActSelect_Unk_ov001_022e0884(w);
    ZoneActSelect_Unk_ov001_022e0858(w, data_ov001_0231c3ec);
    w->timer++;
    if (w->timer >= 0x1e) {
        w->timer = 0;
        w->unk4 = 6;
        data_0208926c.bright = 0;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
        if (w->flags & 1) {
            data_02087d44.bright = 0;
            GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.bright);
        }
        if (gZoneActSelectWork->flags & 8) {
            NaviM_Unk_02064674();
        }
        if (gZoneActSelectWork->flags & 0x10) {
            NaviM_Unk_02064788();
        }
        gZoneActSelectWork->flags &= ~0x18;
        gTaskCurrent->func = ZoneActSelect_State_Select;
    }
}
#pragma thumb reset
