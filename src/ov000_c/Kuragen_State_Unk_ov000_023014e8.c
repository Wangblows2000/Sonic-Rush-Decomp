#include <nitro/types.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x5C - 0x10];
    u16 unk5C;
    u8 pad5E[0x9E - 0x5E];
    u16 unk9E;
    u8 padA0[0xBE - 0xA0];
    s16 unkBE;
    u8 padC0[0x106 - 0xC0];
    s16 unk106;
    u8 pad108[0x238 - 0x108];
    u32 unk238;
    s32 unk23C;
} UnkStruct_023014e8;

void Kuragen_SpawnBomb(UnkStruct_023014e8 *w);
void Kuragen_Action_Unk_ov000_0230187c(UnkStruct_023014e8 *w);

void Kuragen_State_Unk_ov000_023014e8(UnkStruct_023014e8 *w) {
    if (w->unk23C == 0) {
        if (w->unk9E == 4) {
            w->unk5C |= 0x10;
            w->unkC = 1;
            w->unk23C = 1;
        }
    } else if (w->unk23C == 1) {
        w->unkC--;
        if (w->unkC == 0) {
            w->unk5C &= ~0x10;
            w->unkBE <<= 1;
            w->unk106 <<= 1;
            w->unk238 |= 0x20;
            w->unk23C = 2;
        }
    } else if (w->unk23C == 2) {
        if (w->unk9E == 2 && !(w->unk5C & 0x10)) {
            w->unk5C |= 0x10;
            w->unkBE >>= 1;
            w->unk106 >>= 1;
        }
        if ((w->unk238 & 0x40) && w->unk9E == 2) {
            w->unk238 &= ~0x40;
            w->unk238 |= 2;
            w->unkC = 0x3c;
            w->unk23C = 3;
        }
    } else if (w->unk23C == 3) {
        w->unkC--;
        if (w->unkC == 0) {
            w->unk5C &= ~0x10;
            w->unk238 &= ~3;
            w->unk238 |= 0x80;
            Kuragen_SpawnBomb(w);
            w->unk238 |= 0x10000;
            w->unk238 &= ~1;
            Kuragen_Action_Unk_ov000_0230187c(w);
        }
    }
}
