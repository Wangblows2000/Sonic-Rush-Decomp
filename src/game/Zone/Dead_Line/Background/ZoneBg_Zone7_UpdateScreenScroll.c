#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkPair_0233ea38;

typedef struct {
    u8 pad0[0x8];
    UnkPair_0233ea38 a[2];
    UnkPair_0233ea38 b[2];
    UnkPair_0233ea38 c[2];
    u8 pad38[0x48 - 0x38];
    u8 top[2];
    u8 mid[2];
} UnkStruct_0233ea38;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x20 - 0xC];
    s16 unk20;
    u8 pad22[0x2C - 0x22];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_0233ea38;

typedef struct {
    UnkPlayer_0233ea38 p[2];
    u32 unkC8;
} UnkPlayers_0233ea38;

typedef struct {
    u32 unk0;
    u8 pad4[0x64 - 4];
} UnkFlags_0233ea38;

typedef struct {
    u16 unk0;
    u8 pad2[0x64 - 2];
} UnkLimit_0233ea38;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0233ea38_4560;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} UnkStruct_0233ea38_abd8;

extern UnkPlayers_0233ea38 gCamera;
extern UnkFlags_0233ea38 data_ov000_0236ab0c[];
extern UnkLimit_0233ea38 data_ov000_0236ab10[];
extern UnkStruct_0233ea38_4560 gGameState;
extern UnkStruct_0233ea38_abd8 data_ov000_0236abd8;

UnkPlayer_0233ea38 *Camera_GetBySwapState(void);
s32 FX_Div(s32 a, s32 b);

void ZoneBg_Zone7_UpdateScreenScroll(UnkStruct_0233ea38 *bg, s32 idx) {
    UnkPlayer_0233ea38 *p;
    s32 lim;
    s32 h;
    s32 v;
    s32 t;

    if (!(gCamera.unkC8 & 1)) {
        p = &gCamera.p[idx];
    } else {
        idx = 0;
        p = Camera_GetBySwapState();
    }
    if (data_ov000_0236ab0c[idx].unk0 & 0x10) {
        lim = data_ov000_0236ab10[idx].unk0;
    } else {
        lim = 0x7fff;
    }
    h = (s16)((u32)(FX_Div((lim << 8) - p->unk8, p->unk20) >> 4) >> 8);
    if (p->unk34 & 1) {
        return;
    }
    p->unk2C = 0;
    p->unk30 = FX_Div(p->unk8 << 6, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
    bg->a[idx].x = 0;
    bg->a[idx].y = 0;
    bg->b[idx].x = (p->unk4 / 28) & 0x1ffff;
    bg->b[idx].y = FX_Div(p->unk8 << 6, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
    if (h + (bg->b[idx].y >> 8) < 0xb8) {
        bg->top[idx] = h > 0 ? h : 0;
        bg->b[idx].y = (0xb8 - h) << 8;
        t = h + 0x48;
        if (t < 0xc0) {
            if (t <= 0) {
                t = 0;
            }
            bg->mid[idx] = t;
            bg->c[idx].x = (p->unk4 / 14) & 0xffff;
            bg->c[idx].y = ((((p->unk8 / 14) >> 8) & 0xff) + 0x100) << 8;
        } else {
            bg->mid[idx] = 0xc0;
        }
    } else {
        v = 0xb8 - (bg->b[idx].y >> 8);
        if (v < 0) {
            v = 0;
        } else if (v > 0xc0) {
            v = 0xc0;
        }
        bg->top[idx] = v;
        bg->mid[idx] = 0xc0;
    }
}
