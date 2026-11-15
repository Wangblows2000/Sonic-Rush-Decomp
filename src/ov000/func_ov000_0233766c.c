#include <nitro/types.h>

typedef struct UnkStruct_0233766c_2 {
    u8 pad[0xc];
    s32 unkc[1];
} UnkStruct_0233766c_2;

typedef struct UnkStruct_0233766c {
    u8 pad[0x10];
    UnkStruct_0233766c_2 *unk10;
} UnkStruct_0233766c;

extern UnkStruct_0233766c *data_ov000_0236aaa8;

s32 func_ov000_0233766c(s32 i) {
    return data_ov000_0236aaa8->unk10->unkc[i];
}
