#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_02340154_Pos;

typedef struct {
    u8 pad0[0xC];
    UnkStruct_02340154_Pos a[2];
    UnkStruct_02340154_Pos b[2];
    u8 pad2C[0x3C - 0x2C];
    u8 n0[2];
    u8 n1[2];
    u8 n2[2];
} UnkStruct_02340154;

typedef struct {
    u32 flags;
    s32 unk4;
    s32 unk8;
    u8 padC[0x20 - 0xC];
    s16 unk20;
    u8 pad22[0x2C - 0x22];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
    u8 pad38[0x62 - 0x38];
    u16 unk62;
} UnkStruct_02340154_Pl;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} UnkStruct_02340154_abd8;

extern UnkStruct_02340154_Pl gCamera[];
extern UnkStruct_02340154_abd8 data_ov000_0236abd8;

UnkStruct_02340154_Pl *Camera_GetBySwapState(void);
s32 FX_Div(s32 a, s32 b);

void ZoneBg_Zone2_UpdateScreenScroll(UnkStruct_02340154 *w, s32 idx) {
    u8 *out;
    UnkStruct_02340154_Pl *p;
    s32 h;
    s32 v;

    if (gCamera[2].flags & 1) {
        idx = 0;
        p = Camera_GetBySwapState();
        out = &w->n2[0];
    } else {
        p = &gCamera[idx];
        if (idx == 0)
            out = &w->n2[0];
        else
            out = &w->n2[1];
    }
    if (p->unk34 & 1)
        return;

    p->unk2C = 0;
    p->unk30 = FX_Div(p->unk8 * 0x22, (data_ov000_0236abd8.unk10 - 0xC0) << 8) >> 4;
    w->a[idx].x = 0;
    w->a[idx].y = FX_Div(p->unk8 * 0x22, (data_ov000_0236abd8.unk10 - 0xC0) << 8) >> 4;

    if (p->flags & 0x1000000) {
        h = FX_Div((p->unk62 << 8) - p->unk8, p->unk20) >> 4;
        h = (h << 8) >> 16;
        if (h <= 0) {
            w->b[idx].x = (p->unk4 >> 4) & 0xFFFF;
            w->b[idx].y = 0x11A00;
            w->n0[idx] = 0;
            w->n1[idx] = 0;
            *out = 0;
        } else if (h < 0xFC) {
            w->b[idx].x = (p->unk4 >> 4) & 0xFFFF;
            w->b[idx].y = (0x11A - h) << 8;
            v = h - 0x3C;
            if (v < 0)
                v = 0;
            else if (v > 0xC0)
                v = 0xC0;
            w->n0[idx] = v;
            if (h < 0)
                v = 0;
            else if (h > 0xC0)
                v = 0xC0;
            else
                v = h;
            w->n1[idx] = v;
            if (h < 0)
                h = 0;
            else if (h > 0xC0)
                h = 0xC0;
            *out = h;
        } else {
            w->b[idx].x = 0;
            w->b[idx].y = 0;
            w->n0[idx] = 0xC0;
            w->n1[idx] = 0xC0;
            *out = 0xC0;
        }
    } else {
        w->b[idx].x = 0;
        w->b[idx].y = 0;
        w->n0[idx] = 0xC0;
        w->n1[idx] = 0xC0;
        *out = 0xC0;
    }
}
