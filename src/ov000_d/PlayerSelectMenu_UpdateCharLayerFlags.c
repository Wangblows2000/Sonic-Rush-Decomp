#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x758 - 0x18];
    u32 unk758;
    u8 pad75C[0x7A0 - 0x75C];
    u32 unk7A0;
    u8 pad7A4[0x7E8 - 0x7A4];
    u32 unk7E8;
} UnkStruct_02356810;

#pragma thumb on

void PlayerSelectMenu_UpdateCharLayerFlags(UnkStruct_02356810 *w) {
    w->unk758 &= ~0x60000;
    w->unk7A0 &= ~0x60000;
    w->unk7E8 &= ~0x60000;
    switch (w->unk14) {
    case 0:
        w->unk758 |= 0x20000;
        w->unk7A0 |= 0x40000;
        w->unk7E8 |= 0x40000;
        break;
    case 1:
        w->unk758 |= 0x40000;
        w->unk7A0 |= 0x20000;
        w->unk7E8 |= 0x40000;
        break;
    case 2:
        w->unk758 |= 0x40000;
        w->unk7A0 |= 0x40000;
        w->unk7E8 |= 0x20000;
        break;
    }
}

#pragma thumb reset
