#include <nitro/types.h>

typedef struct UnkStruct_02337618_2 {
    u8 pad[0x67c];
    s32 unk67c;
} UnkStruct_02337618_2;

typedef struct UnkStruct_02337618 {
    u8 pad[0x10];
    UnkStruct_02337618_2 *unk10;
} UnkStruct_02337618;

extern UnkStruct_02337618 *data_ov000_0236aaa8;

void func_ov000_02337618(s32 v) {
    if (data_ov000_0236aaa8 != NULL)
        data_ov000_0236aaa8->unk10->unk67c = v;
}
