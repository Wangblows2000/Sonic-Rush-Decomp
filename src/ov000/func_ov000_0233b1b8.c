#include <nitro/types.h>

typedef struct UnkStruct_0233b1b8 {
    u32 unk0;
    u8 pad[0x60];
} UnkStruct_0233b1b8;

extern UnkStruct_0233b1b8 data_ov000_0236aad8[];

void func_ov000_0233b1b8(s32 i) {
    data_ov000_0236aad8[i].unk0 |= 0x40;
}
