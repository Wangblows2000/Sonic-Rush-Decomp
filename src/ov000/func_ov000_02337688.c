#include <nitro/types.h>

typedef struct UnkStruct_02337688 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_02337688;

extern UnkStruct_02337688 *data_ov000_0236aaa8;

void func_ov000_02337688(s32 v) {
    if (data_ov000_0236aaa8 != NULL)
        *data_ov000_0236aaa8->unk10 = v;
}
