#include <nitro/types.h>

typedef struct UnkStruct_02338850_2 {
    u8 pad[0x20];
    s16 unk20;
    u8 pad1[0xc8 - 0x22];
    u32 unkC8;
} UnkStruct_02338850_2;

typedef struct UnkStruct_02338850 {
    u8 pad[8];
    s32 unk8;
} UnkStruct_02338850;

extern UnkStruct_02338850_2 data_ov000_0236aad8;

void func_ov000_02338850(UnkStruct_02338850 *s) {
    data_ov000_0236aad8.unkC8 |= 0x20000;
    s->unk8 = -((data_ov000_0236aad8.unk20 << 12) >> 8);
}
