#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[0x5c - 0x58];
    u16 unk5C;
} UnkStruct_0230d544;

s16 func_02068db4(s32 a, s32 b);

void func_ov000_0230d544(UnkStruct_0230d544 *p) {
    p->unk30 = func_02068db4(p->unk30, 0x20);
    if (p->unkC == 0)
        return;
    p->unkC--;
    if (p->unkC < 0x20) {
        if (p->unkC & 2)
            p->unk5C &= ~0x20;
        else
            p->unk5C |= 0x20;
    }
    if (p->unkC == 0)
        p->unk54 |= 4;
}
