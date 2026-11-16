#include <nitro/types.h>

typedef struct UnkStruct_02301058 {
    u8 pad0[0x8];
    void *objs[5];
} UnkStruct_02301058;

extern UnkStruct_02301058 *gExtraBossPlayerManager;

void func_ov001_02301118(void *o);

void func_ov001_02301058(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (gExtraBossPlayerManager->objs[i] != NULL)
            func_ov001_02301118(gExtraBossPlayerManager->objs[i]);
    }
}
