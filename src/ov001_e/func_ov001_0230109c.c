#include <nitro/types.h>

typedef struct UnkStruct_0230109c {
    u8 pad0[0x44];
    void *objs[0x1e];
} UnkStruct_0230109c;

extern UnkStruct_0230109c *data_ov001_0231f530;

void func_ov001_02301118(void *o);

void func_ov001_0230109c(void) {
    s32 i;

    for (i = 0; i < 0x1e; i++) {
        if (data_ov001_0231f530->objs[i] != NULL)
            func_ov001_02301118(data_ov001_0231f530->objs[i]);
    }
}
