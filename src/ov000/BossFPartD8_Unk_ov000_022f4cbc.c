#include <nitro/types.h>

typedef struct UnkStruct_022f4cbc {
    u8 pad[0x216];
    u16 unk216;
    u8 pad1[0x2c - 0x218 + 0x200];
} UnkStruct_022f4cbc;

void BossFPartD8_Unk_ov000_022f4cbc(u8 *p) {
    s32 i;
    for (i = 0; i < 7; i++) {
        ((UnkStruct_022f4cbc *)p)->unk216 &= ~4;
        p += 0x2c;
    }
}
