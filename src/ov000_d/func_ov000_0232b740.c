#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0232b740_88;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[0x88 - 0x58];
    UnkStruct_0232b740_88 *unk88;
    u8 pad8C[0x134 - 0x8C];
    s16 unk134[5];
    s16 unk13E;
    u8 unk140[4];
    u32 unk144;
    u32 unk148;
} UnkStruct_0232b740;

void func_ov000_02336f28(void *p, u32 v, s32 a);

void func_ov000_0232b740(UnkStruct_0232b740 *w) {
    BOOL done = TRUE;
    s16 i;

    if (w->unk88 != NULL) {
        w->unk10 = w->unk88->unk10;
        w->unk14 = w->unk88->unk14;
    }
    if (w->unk148 != w->unk144) {
        if (((w->unk148 - w->unk144) >> 2) == 0) {
            w->unk144 = w->unk148;
        }
        w->unk144 = w->unk144 + ((w->unk148 - w->unk144) >> 2);
        func_ov000_02336f28(w->unk140, w->unk144, 100000);
    }
    for (i = 0; i < 5; i++) {
        if (w->unk134[i] != 0) {
            w->unk134[i]--;
            done = FALSE;
        }
    }
    if (done) {
        if (w->unk8 != 0) {
            w->unk8 -= 0x10;
            if (w->unk8 < 0x100) {
                w->unk8 = 0;
            }
        }
        if (w->unk148 == w->unk144) {
            w->unkC++;
        }
        if (w->unkC > 0x10) {
            w->unk13E -= 0x180;
        }
    }
    if (w->unk13E < -0x2800) {
        w->unk54 |= 4;
    }
}
