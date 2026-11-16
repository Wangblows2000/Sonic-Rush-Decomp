#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x14C - 0x58];
    void *unk14C;
    u8 pad150[0x1E0 - 0x150];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[4];
    s32 unk1E8;
    s32 unk1EC;
    u8 pad1F0[4];
    u32 unk1F4;
} UnkStruct_0231301c;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

s16 func_02068db4(s16 a, s32 b);
s16 func_02068df8(s16 a, s32 b, s32 c);

void func_ov000_0231301c(UnkStruct_0231301c *w) {
    s32 xmin;
    s32 xmax;
    s32 ymin;
    s32 ymax;
    s16 old;
    s32 v;
    UnkStruct_ObjParam *p;

    if (w->unk34 == 0) {
        if (w->unk14C != NULL)
            return;
        w->unkC++;
        if (!(w->unkC & 1))
            return;
        w->unkC &= ~0xff;
        w->unk54 &= ~2;
        w->unk14C = w;
        return;
    }
    if (!(w->unk1E0->unk4 & 1) && !(w->unk1F4 & 1)) {
        w->unk34 = func_02068db4(w->unk34, 10);
    }
    w->unk2E = (w->unk34 * COS((u16)(w->unkC >> 8))) >> 12;
    w->unk30 = (w->unk34 * SIN((u16)(w->unkC >> 8))) >> 12;
    if (w->unk1E0->unk6 >= 0) {
        xmax = w->unk8;
        xmin = w->unk1E8 << 8;
    } else {
        w->unk30 = -w->unk30;
        w->unk2E = -w->unk2E;
        xmin = w->unk8;
        xmax = w->unk1E8 << 8;
    }
    p = w->unk1E0;
    if (p->unk7 >= 0) {
        ymin = w->unk1EC << 8;
        ymax = (w->unk1EC + (p->unk9 + (p->unk7 << 6))) << 8;
    } else {
        ymin = (w->unk1EC + (p->unk9 + (p->unk7 << 6))) << 8;
        ymax = w->unk1EC << 8;
    }
    if (p->unk4 & 1) {
        if (w->unk30 < 0) {
            old = w->unk34;
            w->unk34 = func_02068db4(old, 0x10);
            v = w->unk34;
            if (v < 0)
                v = -v;
            if (v < 0x40)
                w->unk34 = -old;
        } else if (w->unk34 > 0) {
            w->unk34 = func_02068df8(w->unk34, 0x10, 0xf00);
        } else {
            w->unk34 = func_02068df8(w->unk34, -0x10, 0xf00);
        }
    }
    if ((w->unk2E > 0 && w->unk10 + w->unk2E > xmax) || (w->unk2E < 0 && w->unk10 + w->unk2E < xmin)) {
        w->unk54 |= 2;
    }
    if ((w->unk2E > 0 && w->unk10 >= xmax) || (w->unk2E < 0 && w->unk10 <= xmin)) {
        if (w->unk2E > 0)
            w->unk10 = xmax;
        else
            w->unk10 = xmin;
        w->unk2E = 0;
    }
    if ((w->unk30 > 0 && w->unk14 >= ymax) || (w->unk30 < 0 && w->unk14 <= ymin)) {
        if (w->unk30 > 0)
            w->unk14 = ymax;
        else
            w->unk14 = ymin;
        w->unk30 = 0;
    }
    if (w->unk2E == 0 && w->unk30 == 0)
        w->unk34 = 0;
}
