#include <nitro/types.h>

typedef struct UnkStruct_023376a4 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_023376a4;

extern UnkStruct_023376a4 *data_ov000_0236aaa8;

s32 func_ov000_023376a4(void) {
    if (data_ov000_0236aaa8 != NULL)
        return *data_ov000_0236aaa8->unk10;
    return 0;
}
