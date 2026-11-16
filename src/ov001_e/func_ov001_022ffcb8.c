#include <nitro/types.h>

typedef struct UnkStruct_022ffcb8 {
    u8 pad0[0x22];
    u16 masks[12];
} UnkStruct_022ffcb8;

extern UnkStruct_022ffcb8 *data_ov001_0231f52c;

void func_ov001_022ffcb8(void) {
    s32 i;

    for (i = 0; i < 12; i++)
        data_ov001_0231f52c->masks[i] = 1 << i;
}
