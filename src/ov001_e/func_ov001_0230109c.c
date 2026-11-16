#include <nitro/types.h>

typedef struct UnkStruct_0230109c {
    u8 pad0[0x44];
    void *objs[0x1e];
} UnkStruct_0230109c;

extern UnkStruct_0230109c *gExtraBossPlayerManager;

void func_ov001_02301118(void *o);

void func_ov001_0230109c(void) {
    s32 i;

    for (i = 0; i < 0x1e; i++) {
        if (gExtraBossPlayerManager->objs[i] != NULL)
            func_ov001_02301118(gExtraBossPlayerManager->objs[i]);
    }
}
