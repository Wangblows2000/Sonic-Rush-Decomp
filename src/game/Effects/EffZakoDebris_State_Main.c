#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x2A - 0x10];
    s16 unk2A;
    u8 pad2C[2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
} UnkStruct_0232ddd4;

void EffZakoDebris_State_Main(UnkStruct_0232ddd4 *w) {
    if (w->unkC == 0) {
        if (w->unk58 & 1) {
            w->unk30 = -(w->unk2A >> 1);
            w->unk2E >>= 1;
            w->unk58 |= 0x10;
            w->unkC = 1;
            w->unk62 = 0x40;
            w->unk58 &= ~1;
        }
    } else if ((w->unk58 & 1) && !(w->unk58 & 0x2000)) {
        w->unk62 = 0x40;
        w->unk58 |= 0x2100;
        w->unk2E = 0;
        w->unk30 = 0;
    }
    if (!(w->unk58 & 0x2000))
        return;
    w->unk8++;
    if (w->unk8 > 0x20) {
        if (w->unk8 & 2)
            w->unk5C |= 0x20;
        else
            w->unk5C &= ~0x20;
    }
    if (w->unk8 > 0x30)
        w->unk54 |= 4;
}
