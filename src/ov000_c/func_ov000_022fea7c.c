#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x44 - 0x10];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0xC0 - 0x64];
    u32 unkC0;
    u8 padC4[0x108 - 0xC4];
    u32 unk108;
} UnkStruct_022fea7c;

s16 func_02068bd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_022fe7e8(UnkStruct_022fea7c *w);

void func_ov000_022fea7c(UnkStruct_022fea7c *w) {
    switch (w->unk8) {
    default:
        w->unkC++;
        if (w->unkC & 2)
            w->unk5C &= ~0x20;
        else
            w->unk5C |= 0x20;
        if (w->unk58 & 1)
            w->unk8++;
        break;
    case 0:
        if (w->unkC != 0) {
            w->unkC--;
        } else {
            w->unk44 = func_02068bd8(w->unk44, 0x100, 8, 2, 0, 8);
            w->unk46 = func_02068bd8(w->unk46, 0x100, 0x180, 4, 0, 8);
            w->unk5C &= ~0x20;
        }
        if (w->unk46 == 0x100 && w->unk44 == 0x100) {
            w->unk8++;
            w->unkC = 0x14;
            w->unk62 |= 0x80;
        }
        break;
    case 1:
        w->unkC--;
        if (w->unkC == 0) {
            w->unk8++;
            w->unk58 |= 0x80;
            w->unk58 &= ~0x2000;
            w->unk58 &= ~0x100;
        }
        break;
    }
    if (w->unk8 == 3) {
        w->unk5C &= ~0x20;
        w->unkC0 &= ~0x200;
        w->unk108 &= ~0x200;
        w->unk54 &= ~2;
        w->unkC = 200;
        w->unk8 = 0;
        func_ov000_022fe7e8(w);
    }
}
