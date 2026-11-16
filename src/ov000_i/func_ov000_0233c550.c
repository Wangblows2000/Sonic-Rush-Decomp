#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
} UnkObj_0233c550;

extern UnkObj_0233c550 *gDisplayTable[];

void func_ov000_0233c550(void) {
    u8 i;

    for (i = 0; i < 2; i++) {
        gDisplayTable[i]->unk20 &= ~1;
        gDisplayTable[i]->unk20 |= 2;
        gDisplayTable[i]->unk20 |= 4;
        gDisplayTable[i]->unk20 |= 8;
        gDisplayTable[i]->unk20 &= ~0x10;
        gDisplayTable[i]->unk20 &= ~0x20;
        gDisplayTable[i]->unk20 = (gDisplayTable[i]->unk20 & ~0xc0) | 0xc0;
    }
}
