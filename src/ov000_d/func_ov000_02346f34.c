#include <nitro/types.h>

typedef struct UnkStruct_02346f34 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x34 - 0x18];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x65 - 0x5E];
    s8 unk65;
    u8 pad66[0x330 - 0x66];
    u32 unk330;
    void (*unk334)(struct UnkStruct_02346f34 *w);
} UnkStruct_02346f34;

void func_ov000_0234a064(UnkStruct_02346f34 *w, u8 ang);
s32 func_02066a9c(s32 x, s32 y, u8 a, s32 b, u8 *ang, s32 c);

void func_ov000_02346f34(UnkStruct_02346f34 *w) {
    u8 ang;
    u32 fl;

    if (!(w->unk330 & 1)) {
        return;
    }
    ang = 0;
    fl = w->unk58;
    if (!(fl & 0xf)) {
        return;
    }
    if (fl & 1) {
        func_ov000_0234a064(w, ang);
    } else if (fl & 2) {
        if (w->unk5C & 1) {
            func_ov000_0234a064(w, 0x60);
        } else {
            func_ov000_0234a064(w, 0xa0);
        }
        ang = w->unk1;
        func_02066a9c(w->unk10 >> 8, (w->unk14 >> 8) + w->unk65 - 4, w->unk54 & 1, 3, &ang, 0);
        w->unk1 = ang;
    } else if (fl & 4) {
        if (w->unk5C & 1) {
            func_ov000_0234a064(w, 0x40);
        } else {
            func_ov000_0234a064(w, 0xc0);
        }
    } else if (fl & 8) {
        if (w->unk5C & 1) {
            func_ov000_0234a064(w, 0xc0);
        } else {
            func_ov000_0234a064(w, 0x40);
        }
    }
    if (w->unk330 & 2) {
        w->unk5C ^= 1;
        w->unk34 = -w->unk34;
    }
    w->unk58 |= 1;
    w->unk330 &= ~3;
    w->unk334(w);
}
