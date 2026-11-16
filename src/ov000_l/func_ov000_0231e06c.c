#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_0231e06c_T;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x5C - 0x14];
    u16 unk5C;
    u8 pad5E[0x190 - 0x5E];
    UnkStruct_0231e06c_T *unk190;
    u32 unk194;
} UnkStruct_0231e06c;

void func_ov000_0231df98(UnkStruct_0231e06c *w);
void func_ov000_0231dea0(UnkStruct_0231e06c *w);

void func_ov000_0231e06c(UnkStruct_0231e06c *w) {
    s32 dx;
    s16 r;
    s16 a;
    s16 b;
    s16 d;
    s16 c;

    c = 0;
    r = Rand() & 0xf;
    dx = w->unk190->unk10 - w->unk10;
    if (dx < 0)
        dx = -dx;
    b = 2;
    d = 2;
    a = 12;
    if (w->unk194 & 0x40) {
        if (!(Rand() & 4)) {
            switch (Rand() & 3) {
            case 0:
                w->unk194 &= ~0x10;
                w->unk194 &= ~0x20;
                break;
            case 1:
                w->unk194 |= 0x10;
                w->unk194 &= ~0x20;
                break;
            case 2:
                w->unk194 &= ~0x10;
                w->unk194 |= 0x20;
                break;
            }
        }
    }
    if (w->unk194 & 0x20) {
        if (dx > 0x4000) {
            a = 1;
            b = 10;
            d = 5;
        }
        if (w->unk190->unk10 < w->unk10)
            w->unk5C |= 1;
        else
            w->unk5C &= ~1;
    } else {
        if (Rand() & 1)
            w->unk5C |= 1;
        else
            w->unk5C &= ~1;
    }
    r -= a;
    if (r < 0) {
        func_ov000_0231df98(w);
        return;
    }
    r -= b;
    if (r < 0) {
        w->unkC = (Rand() & 0xf) + 8;
        func_ov000_0231dea0(w);
        return;
    }
    r -= c;
    if (r < 0) {
        w->unkC = (Rand() & 0xf) + 0x1e;
        func_ov000_0231dea0(w);
        return;
    }
    r -= d;
    if (r < 0) {
        w->unkC = (Rand() & 0xf) + 0x10;
        func_ov000_0231dea0(w);
        return;
    }
}
