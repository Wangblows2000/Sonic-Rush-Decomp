#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
} UnkObj_0233c550;

extern UnkObj_0233c550 *data_02072c78[];

void func_ov000_0233c550(void) {
    u8 i;

    for (i = 0; i < 2; i++) {
        data_02072c78[i]->unk20 &= ~1;
        data_02072c78[i]->unk20 |= 2;
        data_02072c78[i]->unk20 |= 4;
        data_02072c78[i]->unk20 |= 8;
        data_02072c78[i]->unk20 &= ~0x10;
        data_02072c78[i]->unk20 &= ~0x20;
        data_02072c78[i]->unk20 = (data_02072c78[i]->unk20 & ~0xc0) | 0xc0;
    }
}
