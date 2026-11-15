#include <nitro/types.h>

typedef struct UnkStruct_02337650_2 {
    u8 pad[0x14];
    s32 unk14[1];
} UnkStruct_02337650_2;

typedef struct UnkStruct_02337650 {
    u8 pad[0x10];
    UnkStruct_02337650_2 *unk10;
} UnkStruct_02337650;

extern UnkStruct_02337650 *data_ov000_0236aaa8;

s32 func_ov000_02337650(s32 i) {
    return data_ov000_0236aaa8->unk10->unk14[i];
}
