#include <nitro/types.h>

typedef struct UnkStruct_0235b0a8 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_0235b0a8;

extern UnkStruct_0235b0a8 *data_ov000_0236ac80;

s32 func_ov000_0235b0a8(void) {
    if (data_ov000_0236ac80 == NULL)
        return 0;
    return *data_ov000_0236ac80->unk10;
}
