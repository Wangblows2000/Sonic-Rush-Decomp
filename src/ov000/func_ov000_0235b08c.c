#include <nitro/types.h>

typedef struct UnkStruct_0235b08c {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_0235b08c;

extern UnkStruct_0235b08c *data_ov000_0236ac80;

void func_ov000_0235b08c(s32 v) {
    if (data_ov000_0236ac80 != NULL)
        *data_ov000_0236ac80->unk10 = v;
}
